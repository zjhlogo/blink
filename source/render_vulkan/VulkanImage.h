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

class VulkanImage
{
public:
    VulkanImage(VulkanLogicalDevice& logicalDevice);
    VulkanImage(VulkanLogicalDevice& logicalDevice, VkImage image);
    ~VulkanImage();

    bool createImage(VkImageType type, uint32_t width, uint32_t height, VkFormat format, VkImageUsageFlags usage);
    void destroyImage();

    bool createImageView(VkFormat format, VkImageAspectFlags aspectFlags);
    void destroyImageView();

    bool createImageMemory();
    void destroyImageMemory();

    operator VkImage () { return m_image; };
    VkImageView getImageView() const { return m_imageView; };

private:
    VulkanLogicalDevice& m_logicalDevice;
    VkImage m_image{};
    VkFormat m_format{VkFormat::VK_FORMAT_UNDEFINED};
    VkImageAspectFlags m_aspectFlags{};

    VkImageView m_imageView{};
    VulkanMemory* m_imageMemory{};

};

NS_END
