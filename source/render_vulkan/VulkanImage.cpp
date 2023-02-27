/*!
 * \file VulkanImage.cpp
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#include "VulkanImage.h"
#include "VulkanBuffer.h"
#include "VulkanCommandBuffer.h"
#include "VulkanLogicalDevice.h"
#include "VulkanMemory.h"

namespace blink
{
    VulkanImage::VulkanImage(VulkanLogicalDevice& logicalDevice, bool generateMipmap)
        : m_logicalDevice(logicalDevice)
        , m_generateMipmap(generateMipmap)
    {
        //
    }

    VulkanImage::VulkanImage(VulkanLogicalDevice& logicalDevice, VkImage image)
        : m_logicalDevice(logicalDevice)
        , m_image(image)
    {
        //
    }

    VulkanImage::~VulkanImage()
    {
        //
        destroyImage();
    }

    VkImage VulkanImage::createImage(VkImageType type, uint32_t width, uint32_t height, VkFormat format, VkImageUsageFlags usage)
    {
        if (m_image != VK_NULL_HANDLE)
        {
            return m_image;
        }

        if (m_generateMipmap)
        {
            m_mipCount = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
        }
        else
        {
            m_mipCount = 1;
        }

        m_imageExtent = {width, height, 1};

        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = type;
        imageInfo.extent = m_imageExtent;
        imageInfo.mipLevels = m_mipCount;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;

        VK_CHECK_RESULT(vkCreateImage((VkDevice)m_logicalDevice, &imageInfo, nullptr, &m_image))
        m_ownedImage = true;
        return m_image;
    }

    void VulkanImage::destroyImage()
    {
        freeImageMemory();

        destroyImageView();

        if (m_ownedImage && m_image != VK_NULL_HANDLE)
        {
            vkDestroyImage((VkDevice)m_logicalDevice, m_image, nullptr);
        }
        m_image = VK_NULL_HANDLE;
    }

    VkImageView VulkanImage::createImageView(VkFormat format, VkImageAspectFlags aspectFlags)
    {
        if (m_imageView != VK_NULL_HANDLE && m_format == format && m_aspectFlags == aspectFlags)
        {
            return m_imageView;
        }

        destroyImageView();

        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = m_image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.components = {
            VK_COMPONENT_SWIZZLE_R,
            VK_COMPONENT_SWIZZLE_G,
            VK_COMPONENT_SWIZZLE_B,
            VK_COMPONENT_SWIZZLE_A,
        };
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = m_mipCount;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;
        VK_CHECK_RESULT(vkCreateImageView((VkDevice)m_logicalDevice, &viewInfo, nullptr, &m_imageView))

        m_format = format;
        m_aspectFlags = aspectFlags;
        return m_imageView;
    }

    void VulkanImage::destroyImageView()
    {
        if (m_imageView != VK_NULL_HANDLE)
        {
            vkDestroyImageView((VkDevice)m_logicalDevice, m_imageView, nullptr);
            m_imageView = VK_NULL_HANDLE;
        }

        m_format = VkFormat::VK_FORMAT_UNDEFINED;
        m_aspectFlags = 0;
    }

    VulkanMemory* VulkanImage::allocateImageMemory()
    {
        if (m_imageMemory != nullptr)
        {
            return m_imageMemory;
        }

        VkMemoryRequirements memRequirements{};
        vkGetImageMemoryRequirements((VkDevice)m_logicalDevice, m_image, &memRequirements);

        m_imageMemory = new VulkanMemory(m_logicalDevice);
        m_imageMemory->allocateMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memRequirements);

        VK_CHECK_RESULT(vkBindImageMemory((VkDevice)m_logicalDevice, (VkImage)m_image, (VkDeviceMemory)*m_imageMemory, 0))
        return m_imageMemory;
    }

    void VulkanImage::freeImageMemory()
    {
        //
        SAFE_DELETE(m_imageMemory);
    }

    void VulkanImage::transitionImageLayout(const VulkanCommandBuffer& commandBuffer,
                                            VkFormat format,
                                            VkImageLayout oldLayout,
                                            VkImageLayout newLayout,
                                            VkAccessFlags srcAccessMask,
                                            VkAccessFlags dstAccessMask,
                                            VkPipelineStageFlags srcStage,
                                            VkPipelineStageFlags dstStage,
                                            uint32_t mipIndex,
                                            uint32_t levelCount)
    {
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = m_image;

        if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
        {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

            if (format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT)
            {
                barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
            }
        }
        else
        {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        }

        barrier.subresourceRange.baseMipLevel = mipIndex;
        barrier.subresourceRange.levelCount = levelCount;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.srcAccessMask = srcAccessMask;
        barrier.dstAccessMask = dstAccessMask;

        vkCmdPipelineBarrier((VkCommandBuffer)commandBuffer, srcStage, dstStage, {}, 0, nullptr, 0, nullptr, 1, &barrier);
    }

    void VulkanImage::copyBufferToImage(const VulkanCommandBuffer& commandBuffer, const VulkanBuffer& stagingBuffer, [[maybe_unused]] VkImageLayout imageLayout)
    {
        VkBufferImageCopy region{};
        region.imageSubresource = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1};
        region.imageExtent = m_imageExtent;

        vkCmdCopyBufferToImage((VkCommandBuffer)commandBuffer, (VkBuffer)stagingBuffer, m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
    }
} // namespace blink
