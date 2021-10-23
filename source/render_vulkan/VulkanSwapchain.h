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
NS_BEGIN

class VulkanWindow;
class VulkanLogicalDevice;
class VulkanImage;
class VulkanTexture;

class VulkanSwapchain
{
public:
    VulkanSwapchain(VulkanWindow& window, VulkanLogicalDevice& logicalDevice);
    ~VulkanSwapchain();

    bool create();
    void destroy();

    operator VkSwapchainKHR() const { return m_swapChain; };
    VkFormat getImageFormat() const { return m_swapChainImageFormat; };
    const VkExtent2D& getImageExtent() const { return m_swapChainExtent; };
    std::size_t getImageCount() const { return m_images.size(); };
    VkFramebuffer getFramebuffers(size_t index) { return m_swapChainFramebuffers[index]; };

    bool recreateSwapChain();

    bool createFramebuffers(VkRenderPass renderPass);
    void destroyFramebuffers();

private:
    bool createSwapChain();
    void destroySwapChain();

    bool createSwapchainImageViews();
    void destroySwapchainImageViews();

private:
    VulkanWindow& m_window;
    VulkanLogicalDevice& m_logicalDevice;

    VkSwapchainKHR m_swapChain{};
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;

    std::vector<VulkanImage*> m_images;
    std::vector<VkFramebuffer> m_swapChainFramebuffers;

    VulkanTexture* m_depthTexture{};

};

NS_END
