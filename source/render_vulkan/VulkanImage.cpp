/*!
 * \file VulkanImage.cpp
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#include "VulkanImage.h"
#include "VulkanLogicalDevice.h"
#include "VulkanMemory.h"

#include <foundation/Log.h>

NS_BEGIN

VulkanImage::VulkanImage(VulkanLogicalDevice& logicalDevice)
    : m_logicalDevice(logicalDevice)
{
}

VulkanImage::VulkanImage(VulkanLogicalDevice& logicalDevice, VkImage image)
    : m_logicalDevice(logicalDevice)
    , m_image(image)
{
}

VulkanImage::~VulkanImage()
{
    destroyImage();
}

bool VulkanImage::createImage(VkImageType type, uint32_t width, uint32_t height, VkFormat format, VkImageUsageFlags usage)
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
        return false;
    }

    return true;
}

void VulkanImage::destroyImage()
{
    destroyImageMemory();

    destroyImageView();

    if (m_image != nullptr)
    {
        vkDestroyImage(m_logicalDevice, m_image, nullptr);
        m_image = nullptr;
    }
}

bool VulkanImage::createImageView(VkFormat format, VkImageAspectFlags aspectFlags)
{
    if (m_imageView != nullptr && m_format == format && m_aspectFlags == aspectFlags) return true;

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
        return false;
    }

    m_format = format;
    m_aspectFlags = aspectFlags;

    return true;
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

bool VulkanImage::createImageMemory()
{
    if (m_imageMemory != nullptr) return false;

    VkMemoryRequirements memRequirements{};
    vkGetImageMemoryRequirements(m_logicalDevice, m_image, &memRequirements);

    m_imageMemory = new VulkanMemory(m_logicalDevice);
    m_imageMemory->allocateMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memRequirements);

    return true;
}

void VulkanImage::destroyImageMemory()
{
    SAFE_DELETE(m_imageMemory);
}

NS_END
