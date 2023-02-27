/*!
 * \file VulkanSwapChain.h
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
    class VulkanFrameBuffer;

    class VulkanSwapChain
    {
    public:
        VulkanSwapChain(VulkanWindow& window, VulkanLogicalDevice& logicalDevice, VulkanRenderPass& renderPass);
        ~VulkanSwapChain();

        VulkanSwapChain(const VulkanSwapChain&) = delete;
        VulkanSwapChain(VulkanSwapChain&&) = delete;
        VulkanSwapChain& operator=(const VulkanSwapChain&) = delete;
        VulkanSwapChain& operator=(VulkanSwapChain&&) = delete;

        bool create();
        bool recreate();

        explicit operator VkSwapchainKHR() const { return m_swapChain; }
        [[nodiscard]] VkFormat getImageFormat() const { return m_swapChainImageFormat; }
        [[nodiscard]] const VkExtent2D& getImageExtent() const { return m_swapChainExtent; }
        [[nodiscard]] std::size_t getImageCount() const { return m_images.size(); }
        VulkanFrameBuffer& getCurrentActiveFrameBuffer() { return *m_swapChainFrameBuffers[m_activeImageIndex]; }

        void setCurrentActiveImageIndex(uint32_t imageIndex) { m_activeImageIndex = imageIndex; }
        [[nodiscard]] uint32_t getCurrentActiveImageIndex() const { return m_activeImageIndex; }

    private:
        bool createSwapChain();
        void destroySwapChain();

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
        VulkanTexture* m_depthTexture{};
        std::vector<VulkanFrameBuffer*> m_swapChainFrameBuffers;

        uint32_t m_activeImageIndex{};
    };
} // namespace blink
