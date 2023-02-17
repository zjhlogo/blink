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
        explicit VulkanImage(VulkanLogicalDevice& logicalDevice, bool generateMipmap = true);
        VulkanImage(VulkanLogicalDevice& logicalDevice, VkImage image);
        ~VulkanImage();

        VulkanImage(const VulkanImage&) = delete;
        VulkanImage(VulkanImage&&) = delete;
        VulkanImage& operator=(const VulkanImage&) = delete;
        VulkanImage& operator=(VulkanImage&&) = delete;

        operator VkImage() { return m_image; }

        VkImage createImage(VkImageType type, uint32_t width, uint32_t height, VkFormat format, VkImageUsageFlags usage);
        void destroyImage();

        VkImageView createImageView(VkFormat format, VkImageAspectFlags aspectFlags);
        void destroyImageView();

        VulkanMemory* allocateImageMemory();
        void freeImageMemory();

        void transitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

        VkImageView getImageView() const { return m_imageView; }

    private:
        VulkanLogicalDevice& m_logicalDevice;
        bool m_generateMipmap{};

        VkImage m_image{};
        VkFormat m_format{VkFormat::VK_FORMAT_UNDEFINED};
        VkImageAspectFlags m_aspectFlags{};
        int m_mipLevels{1};

        VkImageView m_imageView{};
        VulkanMemory* m_imageMemory{};

        bool m_ownedImage{};

    };
} // namespace blink
