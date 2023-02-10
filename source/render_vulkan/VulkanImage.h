/*!
 * \file VulkanImage.h
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#pragma once
#include "VulkanBase.h"

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanMemory;
    class VulkanCommandPool;

    class VulkanImage
    {
    public:
        explicit VulkanImage(VulkanLogicalDevice& logicalDevice);
        VulkanImage(VulkanLogicalDevice& logicalDevice, VkImage image);
        ~VulkanImage();

        VulkanImage(const VulkanImage& image) = delete;
        VulkanImage(VulkanImage&& image) = delete;
        VulkanImage& operator=(const VulkanImage& image) = delete;
        VulkanImage& operator=(VulkanImage&& image) = delete;

        operator VkImage() { return m_image; }

        VkImage createImage(VkImageType type, uint32_t width, uint32_t height, VkFormat format, VkImageUsageFlags usage);
        void destroyImage(bool destroyImageHandle = true);

        VkImageView createImageView(VkFormat format, VkImageAspectFlags aspectFlags);
        void destroyImageView();

        VulkanMemory* allocateImageMemory();
        void freeImageMemory();

        void transitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

        VkImageView getImageView() const { return m_imageView; }

    private:
        VulkanLogicalDevice& m_logicalDevice;

        VkImage m_image{};
        VkFormat m_format{VkFormat::VK_FORMAT_UNDEFINED};
        VkImageAspectFlags m_aspectFlags{};

        VkImageView m_imageView{};
        VulkanMemory* m_imageMemory{};
    };
} // namespace blink
