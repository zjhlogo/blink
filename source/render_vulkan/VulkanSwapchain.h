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
#include <foundation/BaseTypesGlm.h>
#include <vulkan/vulkan.h>

#include <vector>
NS_BEGIN

class VulkanWindow;
class VulkanContext;
class VulkanLogicalDevice;
class VulkanImage;

class VulkanSwapchain
{
public:
    VulkanSwapchain();
    ~VulkanSwapchain();

    bool initialize(VulkanWindow* window, VulkanContext* context, VkDevice logicalDevice);
    void terminate();

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
    VulkanWindow* m_window{};
    VulkanContext* m_context{};
    VkDevice m_logicalDevice{};

    VkSwapchainKHR m_swapChain;
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;

    std::vector<VulkanImage*> m_images;
    std::vector<VkFramebuffer> m_swapChainFramebuffers;
};

NS_END
