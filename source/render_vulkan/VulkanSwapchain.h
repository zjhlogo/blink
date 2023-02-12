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

    class VulkanSwapchain
    {
    public:
        VulkanSwapchain(VulkanWindow& window, VulkanLogicalDevice& logicalDevice);
        ~VulkanSwapchain();

        VulkanSwapchain(const VulkanSwapchain& swapchain) = delete;
        VulkanSwapchain(VulkanSwapchain&& swapchain) = delete;
        VulkanSwapchain& operator=(const VulkanSwapchain& swapchain) = delete;
        VulkanSwapchain& operator=(VulkanSwapchain&& swapchain) = delete;

        bool create();
        bool recreate();
        void destroy();

        operator VkSwapchainKHR() const { return m_swapChain; }
        VkFormat getImageFormat() const { return m_swapChainImageFormat; }
        const VkExtent2D& getImageExtent() const { return m_swapChainExtent; }
        std::size_t getImageCount() const { return m_images.size(); }
        VkFramebuffer getCurrentActiveFrameBuffer() { return m_swapChainFrameBuffers[m_activeImageIndex]; }
        VkRenderPass getRenderPass() const { return m_renderPass; }

        void setCurrentActiveImageIndex(uint32_t imageIndex) { m_activeImageIndex = imageIndex; }
        uint32_t getCurrentActiveImageIndex() { return m_activeImageIndex; }

    private:
        bool createSwapChain();
        void destroySwapChain();

        bool createSwapchainImageViews();
        void destroySwapchainImageViews();

        VkRenderPass createRenderPass(VkFormat colorAttachmentFormat, VkFormat depthAttachmentFormat);
        void destroyRenderPass();

        bool createFrameBuffers(VkRenderPass renderPass);
        void destroyFrameBuffers();

    private:
        VulkanWindow& m_window;
        VulkanLogicalDevice& m_logicalDevice;

        VkSwapchainKHR m_swapChain{};
        VkFormat m_swapChainImageFormat{};
        VkExtent2D m_swapChainExtent{};

        std::vector<VulkanImage*> m_images;
        std::vector<VkFramebuffer> m_swapChainFrameBuffers;

        VulkanTexture* m_depthTexture{};

        VkRenderPass m_renderPass{};

        uint32_t m_activeImageIndex{};
    };
} // namespace blink
