/*!
 * \file VulkanImage.cpp
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#include "VulkanImage.h"
#include "VulkanCommandBuffer.h"
#include "VulkanCommandPool.h"
#include "VulkanLogicalDevice.h"
#include "VulkanMemory.h"

#include <foundation/Log.h>

namespace blink
{
    VulkanImage::VulkanImage(VulkanLogicalDevice& logicalDevice)
        : m_logicalDevice(logicalDevice)
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
        destroyImage();

        VkImageCreateInfo imageInfo{};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = type;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = usage;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;

        if (vkCreateImage(m_logicalDevice, &imageInfo, nullptr, &m_image) != VK_SUCCESS)
        {
            LOGE("create image failed");
            return nullptr;
        }

        return m_image;
    }

    void VulkanImage::destroyImage(bool destroyImageHandle)
    {
        freeImageMemory();

        destroyImageView();

        if (destroyImageHandle && m_image != nullptr)
        {
            vkDestroyImage(m_logicalDevice, m_image, nullptr);
        }
        m_image = nullptr;
    }

    VkImageView VulkanImage::createImageView(VkFormat format, VkImageAspectFlags aspectFlags)
    {
        if (m_imageView != nullptr && m_format == format && m_aspectFlags == aspectFlags)
        {
            return m_imageView;
        }

        destroyImageView();

        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = m_image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;
        if (vkCreateImageView(m_logicalDevice, &viewInfo, nullptr, &m_imageView) != VK_SUCCESS)
        {
            LOGE("create image view failed");
            return nullptr;
        }

        m_format = format;
        m_aspectFlags = aspectFlags;

        return m_imageView;
    }

    void VulkanImage::destroyImageView()
    {
        if (m_imageView != nullptr)
        {
            vkDestroyImageView(m_logicalDevice, m_imageView, nullptr);
            m_imageView = nullptr;
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
        vkGetImageMemoryRequirements(m_logicalDevice, m_image, &memRequirements);

        m_imageMemory = new VulkanMemory(m_logicalDevice);
        m_imageMemory->allocateMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memRequirements);

        if (vkBindImageMemory(m_logicalDevice, m_image, *m_imageMemory, 0) != VK_SUCCESS)
        {
            LOGE("bind image memory failed");
            return nullptr;
        }

        return m_imageMemory;
    }

    void VulkanImage::freeImageMemory()
    {
        //
        SAFE_DELETE(m_imageMemory);
    }

    void VulkanImage::transitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
    {
        m_logicalDevice.executeCommand(
            [&](VulkanCommandBuffer& commandBuffer)
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

                barrier.subresourceRange.baseMipLevel = 0;
                barrier.subresourceRange.levelCount = 1;
                barrier.subresourceRange.baseArrayLayer = 0;
                barrier.subresourceRange.layerCount = 1;

                VkPipelineStageFlags sourceStage{};
                VkPipelineStageFlags destinationStage{};

                if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
                {
                    barrier.srcAccessMask = {};
                    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

                    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                    destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                }
                else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
                {
                    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

                    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                }
                else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
                {
                    barrier.srcAccessMask = {};
                    barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

                    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                    destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
                }

                vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, {}, 0, nullptr, 0, nullptr, 1, &barrier);
            });
    }

} // namespace blink
