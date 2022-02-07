/*!
 * \file VulkanSwapchain.h
 *
 * \author zjhlogo
 * \date 2020/04/11
 *
 *
 */
#pragma once
#include <foundation/BaseTypes.h>
#include <vulkan/vulkan.h>

#include <vector>
namespace blink
{
    class VulkanWindow;
    class VulkanLogicalDevice;
    class VulkanCommandPool;
    class VulkanImage;
    class VulkanTexture;

    class VulkanSwapchain
    {
    public:
        VulkanSwapchain(VulkanWindow& window, VulkanLogicalDevice& logicalDevice, VulkanCommandPool& commandPool);
        ~VulkanSwapchain();

        bool create();
        bool recreate();
        void destroy();

        operator VkSwapchainKHR() const { return m_swapChain; };
        VkFormat getImageFormat() const { return m_swapChainImageFormat; };
        const VkExtent2D& getImageExtent() const { return m_swapChainExtent; };
        std::size_t getImageCount() const { return m_images.size(); };
        VkFramebuffer getFramebuffers(size_t index) { return m_swapChainFramebuffers[index]; };
        VkRenderPass getRenderPass() const { return m_renderPass; };

        void setCurrentActivedImageIndex(uint32_t imageIndex) { m_activedImageIndex = imageIndex; };
        uint32_t getCurrentActivedImageIndex() { return m_activedImageIndex; };

    private:
        bool createSwapChain();
        void destroySwapChain();

        bool createSwapchainImageViews();
        void destroySwapchainImageViews();

        VkRenderPass createRenderPass(VkFormat colorAttachmentFormat, VkFormat depthAttachmentFormat);
        void destroyRenderPass();

        bool createFramebuffers(VkRenderPass renderPass);
        void destroyFramebuffers();

    private:
        VulkanWindow& m_window;
        VulkanLogicalDevice& m_logicalDevice;
        VulkanCommandPool& m_commandPool;

        VkSwapchainKHR m_swapChain{};
        VkFormat m_swapChainImageFormat;
        VkExtent2D m_swapChainExtent;

        std::vector<VulkanImage*> m_images;
        std::vector<VkFramebuffer> m_swapChainFramebuffers;

        VulkanTexture* m_depthTexture{};

        VkRenderPass m_renderPass{};

        uint32_t m_activedImageIndex{};
    };

} // namespace blink
