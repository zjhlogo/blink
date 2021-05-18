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
#include <vulkan/vulkan.hpp>

NS_BEGIN

class VulkanWindow;
class VulkanContext;
class VulkanLogicalDevice;

class VulkanSwapchain
{
public:
    VulkanSwapchain();
    ~VulkanSwapchain();

    bool initialize(VulkanWindow* window, VulkanContext* context, VulkanLogicalDevice* logicalDevice);
    void terminate();

    vk::Format getImageFormat() const { return m_swapChainImageFormat; };
    const vk::Extent2D& getImageExtent() const { return m_swapChainExtent; };
    std::size_t getImageCount() const { return m_swapChainImages.size(); };

//     bool recreateSwapChain();

private:
    bool createSwapChain();
    void destroySwapChain();

    bool createSwapchainImageViews();
    void destroySwapchainImageViews();

private:
    VulkanWindow* m_window{};
    VulkanContext* m_context{};
    VulkanLogicalDevice* m_logicalDevice{};

    vk::SwapchainKHR m_swapChain;
    std::vector<vk::Image> m_swapChainImages;
    std::vector<vk::ImageView> m_swapChainImageViews;

    vk::Format m_swapChainImageFormat;
    vk::Extent2D m_swapChainExtent;

};

NS_END
