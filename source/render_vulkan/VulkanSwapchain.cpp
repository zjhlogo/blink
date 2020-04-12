/*!
 * \file VulkanSwapchain.cpp
 *
 * \author zjhlogo
 * \date 2020/04/11
 *
 *
 */
#include "VulkanSwapchain.h"
#include "VulkanContext.h"
#include "VulkanLogicalDevice.h"
#include "VulkanWindow.h"
#include "utils/VulkanUtils.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

NS_BEGIN

VulkanSwapchain::VulkanSwapchain()
{
}

VulkanSwapchain::~VulkanSwapchain()
{
}

bool VulkanSwapchain::initialize(VulkanWindow* window, VulkanContext* context, VulkanLogicalDevice* logicalDevice)
{
    m_window = window;
    m_context = context;
    m_logicalDevice = logicalDevice;

    if (!createSwapChain()) return false;
    if (!createSwapchainImageViews()) return false;

    return true;
}

void VulkanSwapchain::terminate()
{
    destroySwapchainImageViews();
    destroySwapChain();
}

// bool VulkanSwapchain::recreateSwapChain()
// {
//     int width = 0;
//     int height = 0;
//     glfwGetFramebufferSize(m_window->getWindow(), &width, &height);
// 
//     while (width == 0 || height == 0)
//     {
//         glfwGetFramebufferSize(m_window->getWindow(), &width, &height);
//         glfwWaitEvents();
//     }
// 
//     m_logicalDevice->getVkLogicalDevice().waitIdle();
// 
//     destroySwapchainImageViews();
//     destroySwapChain();
// 
//     if (!createSwapChain()) return false;
//     if (!createSwapchainImageViews()) return false;
// 
//     return true;
// }

bool VulkanSwapchain::createSwapChain()
{
    const auto& physicalDevice = m_context->getPickedPhysicalDevice();
    const auto& surface = m_context->getVkSurface();
    const auto& logicalDevice = m_logicalDevice->getVkLogicalDevice();

    // select format
    std::vector<vk::SurfaceFormatKHR> formats = physicalDevice.getSurfaceFormatsKHR(surface);
    vk::SurfaceFormatKHR selFormat = formats[0];
    for (const auto& format : formats)
    {
        if (format.format == vk::Format::eB8G8R8A8Unorm && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
        {
            selFormat = format;
            break;
        }
    }

    // select present mode
    std::vector<vk::PresentModeKHR> presentModes = physicalDevice.getSurfacePresentModesKHR(surface);
    vk::PresentModeKHR selPresentMode = vk::PresentModeKHR::eFifo;
    for (const auto& presentMode : presentModes)
    {
        if (presentMode == vk::PresentModeKHR::eMailbox)
        {
            selPresentMode = presentMode;
            break;
        }
        else if (presentMode == vk::PresentModeKHR::eImmediate)
        {
            selPresentMode = presentMode;
        }
    }

    // select extent
    vk::SurfaceCapabilitiesKHR capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
    vk::Extent2D selExtent;
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        selExtent = capabilities.currentExtent;
    }
    else
    {
        int width, height;
        glfwGetFramebufferSize(m_window->getWindow(), &width, &height);
        vk::Extent2D actualExtent(width, height);
        selExtent = actualExtent;
    }

    // select image count
    uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
    {
        imageCount = capabilities.maxImageCount;
    }

    vk::SwapchainCreateInfoKHR createInfo;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = selFormat.format;
    createInfo.imageColorSpace = selFormat.colorSpace;
    createInfo.imageExtent = selExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

    // get best fit queue index from queue families
    int graphicsFamilyIndex{};
    int presentFamilyIndex{};
    VulkanUtils::getBestFitQueueFamilyPropertyIndex(graphicsFamilyIndex,
                                                    presentFamilyIndex,
                                                    physicalDevice,
                                                    surface,
                                                    physicalDevice.getQueueFamilyProperties());
    if (graphicsFamilyIndex != presentFamilyIndex)
    {
        uint32_t queueFamilyIndexs[2] = {static_cast<uint32_t>(graphicsFamilyIndex), static_cast<uint32_t>(presentFamilyIndex)};
        createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndexs;
    }
    else
    {
        createInfo.imageSharingMode = vk::SharingMode::eExclusive;
    }

    createInfo.preTransform = capabilities.currentTransform;
    createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    createInfo.presentMode = selPresentMode;
    createInfo.clipped = VK_TRUE;

    m_swapChain = logicalDevice.createSwapchainKHR(createInfo);
    m_swapChainImages = logicalDevice.getSwapchainImagesKHR(m_swapChain);

    m_swapChainImageFormat = selFormat.format;
    m_swapChainExtent = selExtent;

    return true;
}

void VulkanSwapchain::destroySwapChain()
{
    const auto& logicalDevice = m_logicalDevice->getVkLogicalDevice();
    logicalDevice.destroySwapchainKHR(m_swapChain);
}

bool VulkanSwapchain::createSwapchainImageViews()
{
    m_swapChainImageViews.resize(m_swapChainImages.size());

    for (size_t i = 0; i < m_swapChainImageViews.size(); ++i)
    {
        m_swapChainImageViews[i] = m_logicalDevice->createImageView(m_swapChainImages[i], m_swapChainImageFormat, vk::ImageAspectFlagBits::eColor);
    }

    return true;
}

void VulkanSwapchain::destroySwapchainImageViews()
{
    const auto& logicalDevice = m_logicalDevice->getVkLogicalDevice();

    for (auto& imageView : m_swapChainImageViews)
    {
        logicalDevice.destroyImageView(imageView);
    }
    m_swapChainImageViews.clear();
}

NS_END
