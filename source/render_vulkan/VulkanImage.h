/*!
 * \file VulkanImage.h
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#pragma once
#include <foundation/BaseTypes.h>
#include <vulkan/vulkan.h>

NS_BEGIN

class VulkanLogicalDevice;
class VulkanMemory;
class VulkanCommandPool;

class VulkanImage
{
public:
    VulkanImage(VulkanLogicalDevice& logicalDevice);
    VulkanImage(VulkanLogicalDevice& logicalDevice, VkImage image);
    ~VulkanImage();

    operator VkImage() { return m_image; };

    VkImage createImage(VkImageType type, uint32_t width, uint32_t height, VkFormat format, VkImageUsageFlags usage);
    void destroyImage(bool destroyImageHandle = true);

    VkImageView createImageView(VkFormat format, VkImageAspectFlags aspectFlags);
    void destroyImageView();

    VulkanMemory* allocateImageMemory();
    void freeImageMemory();

    void transitionImageLayout(VulkanCommandPool& pool, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    VkImageView getImageView() const { return m_imageView; };

private:
    VulkanLogicalDevice& m_logicalDevice;
    VkImage m_image{ VK_NULL_HANDLE };
    VkFormat m_format{VkFormat::VK_FORMAT_UNDEFINED};
    VkImageAspectFlags m_aspectFlags{};

    VkImageView m_imageView{ VK_NULL_HANDLE };
    VulkanMemory* m_imageMemory{};

};

NS_END
