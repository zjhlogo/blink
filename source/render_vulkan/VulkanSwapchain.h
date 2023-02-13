/*!
 * \file VulkanSwapchain.h
 *
 * \author zjhlogo
 * \date 2020/04/11
 *
 *
 */
#pragma once
#include "VulkanBase.h"

#include <vector>

namespace blink
{
    class VulkanWindow;
    class VulkanLogicalDevice;
    class VulkanImage;
    class VulkanTexture;
    class VulkanRenderPass;

    class VulkanSwapchain
    {
    public:
        VulkanSwapchain(VulkanWindow& window, VulkanLogicalDevice& logicalDevice, VulkanRenderPass& renderPass);
        ~VulkanSwapchain();

        VulkanSwapchain(const VulkanSwapchain&) = delete;
        VulkanSwapchain(VulkanSwapchain&&) = delete;
        VulkanSwapchain& operator=(const VulkanSwapchain&) = delete;
        VulkanSwapchain& operator=(VulkanSwapchain&&) = delete;

        bool create();
        bool recreate();
        void destroy();

        operator VkSwapchainKHR() const { return m_swapChain; }
        VkFormat getImageFormat() const { return m_swapChainImageFormat; }
        const VkExtent2D& getImageExtent() const { return m_swapChainExtent; }
        std::size_t getImageCount() const { return m_images.size(); }
        VkFramebuffer getCurrentActiveFrameBuffer() { return m_swapChainFrameBuffers[m_activeImageIndex]; }

        void setCurrentActiveImageIndex(uint32_t imageIndex) { m_activeImageIndex = imageIndex; }
        uint32_t getCurrentActiveImageIndex() { return m_activeImageIndex; }

    private:
        bool createSwapChain();
        void destroySwapChain();

        bool createSwapChainImageViews();
        void destroySwapChainImageViews();

        bool createFrameBuffers();
        void destroyFrameBuffers();

    private:
        VulkanWindow& m_window;
        VulkanLogicalDevice& m_logicalDevice;
        VulkanRenderPass& m_renderPass;

        VkSwapchainKHR m_swapChain{};
        VkFormat m_swapChainImageFormat{};
        VkExtent2D m_swapChainExtent{};

        std::vector<VulkanImage*> m_images;
        std::vector<VkFramebuffer> m_swapChainFrameBuffers;

        VulkanTexture* m_depthTexture{};

        uint32_t m_activeImageIndex{};
    };
} // namespace blink
