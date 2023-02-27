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
    class VulkanBuffer;
    class VulkanCommandPool;
    class VulkanCommandBuffer;

    class VulkanImage
    {
    public:
        explicit VulkanImage(VulkanLogicalDevice& logicalDevice, bool generateMipmap);
        VulkanImage(VulkanLogicalDevice& logicalDevice, VkImage image);
        ~VulkanImage();

        VulkanImage(const VulkanImage&) = delete;
        VulkanImage(VulkanImage&&) = delete;
        VulkanImage& operator=(const VulkanImage&) = delete;
        VulkanImage& operator=(VulkanImage&&) = delete;

        explicit operator VkImage() { return m_image; }

        VkImage createImage(VkImageType type, uint32_t width, uint32_t height, VkFormat format, VkImageUsageFlags usage);
        void destroyImage();

        VkImageView createImageView(VkFormat format, VkImageAspectFlags aspectFlags);
        void destroyImageView();

        VulkanMemory* allocateImageMemory();
        void freeImageMemory();

        void transitionImageLayout(const VulkanCommandBuffer& commandBuffer,
                                   VkFormat format,
                                   VkImageLayout oldLayout,
                                   VkImageLayout newLayout,
                                   VkAccessFlags srcAccessMask,
                                   VkAccessFlags dstAccessMask,
                                   VkPipelineStageFlags srcStage,
                                   VkPipelineStageFlags dstStage,
                                   uint32_t mipIndex,
                                   uint32_t levelCount = 1);
        void copyBufferToImage(const VulkanCommandBuffer& commandBuffer, const VulkanBuffer& stagingBuffer, [[maybe_unused]] VkImageLayout mipIndex);

        [[nodiscard]] VkImageView getImageView() const { return m_imageView; }
        [[nodiscard]] uint32_t getMipCount() const { return m_mipCount; }

    private:
        VulkanLogicalDevice& m_logicalDevice;
        bool m_generateMipmap{};

        VkImage m_image{};
        VkFormat m_format{VkFormat::VK_FORMAT_UNDEFINED};
        VkImageAspectFlags m_aspectFlags{};
        uint32_t m_mipCount{1};
        VkExtent3D m_imageExtent{};

        VkImageView m_imageView{};
        VulkanMemory* m_imageMemory{};

        bool m_ownedImage{};
    };
} // namespace blink
