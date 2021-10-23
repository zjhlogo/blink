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
#include "VulkanLogicalDevice.h"
#include "VulkanTexture.h"
#include "VulkanWindow.h"
#include "utils/VulkanUtils.h"

#include <foundation/Log.h>

#include <array>

#define GLFW_INCLUDE_VULKAN
#include <glfw3/glfw3.h>

NS_BEGIN

VulkanSwapchain::VulkanSwapchain(VulkanWindow& window, VulkanLogicalDevice& logicalDevice, VulkanCommandPool& commandPool)
    : m_window(window)
    , m_logicalDevice(logicalDevice)
    , m_commandPool(commandPool)
{
}

VulkanSwapchain::~VulkanSwapchain()
{
    destroy();
}

bool VulkanSwapchain::create()
{
    if (!createSwapChain()) return false;
    if (!createSwapchainImageViews()) return false;

    return true;
}

void VulkanSwapchain::destroy()
{
    destroySwapchainImageViews();
    destroySwapChain();
}

bool VulkanSwapchain::createFramebuffers(VkRenderPass renderPass)
{
    SAFE_DELETE(m_depthTexture);

    m_depthTexture = new VulkanTexture(m_logicalDevice, m_commandPool);
    if (!m_depthTexture->createDepthTexture(m_swapChainExtent.width, m_swapChainExtent.height)) return false;

    auto count = m_images.size();
    m_swapChainFramebuffers.resize(count);

    for (size_t i = 0; i < count; ++i)
    {
        auto imageView = m_images[i]->getImageView();

        std::array<VkImageView, 2> attacments = {imageView, m_depthTexture->getTextureImage()->getImageView()};

        VkFramebufferCreateInfo frameBufferInfo{};
        frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frameBufferInfo.renderPass = renderPass;
        frameBufferInfo.attachmentCount = static_cast<uint32_t>(attacments.size());
        frameBufferInfo.pAttachments = attacments.data();
        frameBufferInfo.width = m_swapChainExtent.width;
        frameBufferInfo.height = m_swapChainExtent.height;
        frameBufferInfo.layers = 1;
        if (vkCreateFramebuffer(m_logicalDevice, &frameBufferInfo, nullptr, &m_swapChainFramebuffers[i]) != VK_SUCCESS)
        {
            LOGE("create swapchain frame buffer failed");
        }
    }

    return true;
}

void VulkanSwapchain::destroyFramebuffers()
{
    for (auto framebuffer : m_swapChainFramebuffers)
    {
        vkDestroyFramebuffer(m_logicalDevice, framebuffer, nullptr);
    }
    m_swapChainFramebuffers.clear();

    SAFE_DELETE(m_depthTexture);
}

bool VulkanSwapchain::recreateSwapChain()
{
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);

    while (width == 0 || height == 0)
    {
        glfwGetFramebufferSize(m_window, &width, &height);
        glfwWaitEvents();
    }

    m_logicalDevice.waitIdle();

    destroySwapchainImageViews();
    destroySwapChain();

    if (!createSwapChain()) return false;
    if (!createSwapchainImageViews()) return false;

    return true;
}

bool VulkanSwapchain::createSwapChain()
{
    auto context = m_logicalDevice.getContext();
    auto physicalDevice = context->getPickedPhysicalDevice();
    auto surface = context->getVkSurface();

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

    VkExtent2D selExtent;
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        selExtent = capabilities.currentExtent;
    }
    else
    {
        int width, height;
        glfwGetFramebufferSize(*context->getWindow(), &width, &height);
        selExtent.width = width;
        selExtent.height = height;
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
