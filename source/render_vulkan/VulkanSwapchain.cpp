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
#include "VulkanImage.h"
#include "VulkanTexture.h"
#include "VulkanWindow.h"
#include "utils/VulkanUtils.h"

#include <foundation/Log.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

NS_BEGIN

VulkanSwapchain::VulkanSwapchain()
{
}

VulkanSwapchain::~VulkanSwapchain()
{
}

bool VulkanSwapchain::initialize(VulkanWindow* window, VulkanContext* context, VkDevice logicalDevice)
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

bool VulkanSwapchain::createFramebuffers(VulkanTexture* depthTexture, VkRenderPass renderPass)
{
    m_swapChainFramebuffers.resize(m_swapChainImageViews.size());

    for (size_t i = 0; i < m_swapChainImageViews.size(); ++i)
    {
        m_swapChainFramebuffers[i] = m_logicalDevice->createFramebuffer(m_swapChainImageViews[i], depthTexture, renderPass, m_swapChainExtent);
    }

    return true;
}

void VulkanSwapchain::destroyFramebuffers()
{
    for (auto framebuffer : m_swapChainFramebuffers)
    {
        m_logicalDevice->destroyFramebuffer(framebuffer);
    }
    m_swapChainFramebuffers.clear();
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
    auto physicalDevice = m_context->getPickedPhysicalDevice();
    auto surface = m_context->getVkSurface();

    // select format
    std::vector<VkSurfaceFormatKHR> formats;
    VulkanUtils::getSurfaceFormats(formats, physicalDevice, surface);

    VkSurfaceFormatKHR selFormat = formats[0];
    for (const auto& format : formats)
    {
        if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            selFormat = format;
            break;
        }
    }

    // select present mode
    std::vector<VkPresentModeKHR> presentModes;
    VulkanUtils::getSurfacePresentModes(presentModes, physicalDevice, surface);

    VkPresentModeKHR selPresentMode = VK_PRESENT_MODE_FIFO_KHR;
    for (const auto& presentMode : presentModes)
    {
        if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            selPresentMode = presentMode;
            break;
        }
        else if (presentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
        {
            selPresentMode = presentMode;
        }
    }

    // select extent
    VkSurfaceCapabilitiesKHR capabilities{};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);

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

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = selFormat.format;
    createInfo.imageColorSpace = selFormat.colorSpace;
    createInfo.imageExtent = selExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    // get best fit queue index from queue families
    int graphicsFamilyIndex{};
    int presentFamilyIndex{};

    std::vector<VkQueueFamilyProperties> queueFamilies;
    VulkanUtils::getPhysicalDeviceQueueFamilyProperties(queueFamilies, physicalDevice);
    VulkanUtils::getBestFitQueueFamilyPropertyIndex(graphicsFamilyIndex, presentFamilyIndex, physicalDevice, surface, queueFamilies);

    if (graphicsFamilyIndex != presentFamilyIndex)
    {
        uint32_t queueFamilyIndexs[2] = {static_cast<uint32_t>(graphicsFamilyIndex), static_cast<uint32_t>(presentFamilyIndex)};
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndexs;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = selPresentMode;
    createInfo.clipped = VK_TRUE;

    if (vkCreateSwapchainKHR(m_logicalDevice, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
    {
        LOGE("create swapchain failed");
        return false;
    }

    uint32_t swapchainImageCount = 0;
    vkGetSwapchainImagesKHR(m_logicalDevice, m_swapChain, &swapchainImageCount, nullptr);

    std::vector<VkImage> swapChainImages;
    swapChainImages.resize(swapchainImageCount);
    vkGetSwapchainImagesKHR(m_logicalDevice, m_swapChain, &swapchainImageCount, swapChainImages.data());
    m_swapChainImageFormat = selFormat.format;
    m_swapChainExtent = selExtent;

    m_images.clear();
    for (auto image : swapChainImages)
    {
        auto vulkanImage = new VulkanImage(m_logicalDevice, image);
        m_images.push_back(vulkanImage);
    }

    return true;
}

void VulkanSwapchain::destroySwapChain()
{
    vkDestroySwapchainKHR(m_logicalDevice, m_swapChain, nullptr);
}

bool VulkanSwapchain::createSwapchainImageViews()
{
    for (auto vulkanImage : m_images)
    {
        vulkanImage->createImageView(m_swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
    }

    return true;
}

void VulkanSwapchain::destroySwapchainImageViews()
{
    for (auto vulkanImage : m_images)
    {
        vulkanImage->destroyImageView();
    }
}

NS_END
