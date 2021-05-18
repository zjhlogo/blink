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

class VulkanLogicalDevice;
class VulkanImage;
class VulkanTexture;

class VulkanSwapchain
{
public:
    VulkanSwapchain(VulkanLogicalDevice& logicalDevice);
    ~VulkanSwapchain();

    bool create();
    void destroy();

    VkFormat getImageFormat() const { return m_swapChainImageFormat; };
    const VkExtent2D& getImageExtent() const { return m_swapChainExtent; };
    std::size_t getImageCount() const { return m_images.size(); };

    //     bool recreateSwapChain();

    bool createFramebuffers(VulkanTexture* depthTexture, VkRenderPass renderPass);
    void destroyFramebuffers();

private:
    bool createSwapChain();
    void destroySwapChain();

    bool createSwapchainImageViews();
    void destroySwapchainImageViews();

private:
    VulkanLogicalDevice& m_logicalDevice;

    VkSwapchainKHR m_swapChain{};
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;

    std::vector<VulkanImage*> m_images;
    std::vector<VkFramebuffer> m_swapChainFramebuffers;
};

NS_END
