/*!
 * \file VulkanSwapChain.cpp
 *
 * \author zjhlogo
 * \date 2020/04/11
 *
 *
 */
#include "VulkanSwapChain.h"
#include "VulkanContext.h"
#include "VulkanFrameBuffer.h"
#include "VulkanImage.h"
#include "VulkanLogicalDevice.h"
#include "VulkanRenderPass.h"
#include "VulkanWindow.h"
#include "resources/VulkanTexture.h"
#include "utils/VulkanUtils.h"

#define GLFW_INCLUDE_VULKAN
#include <glfw3/glfw3.h>

namespace blink
{
    VulkanSwapChain::VulkanSwapChain(VulkanWindow& window, VulkanLogicalDevice& logicalDevice, VulkanRenderPass& renderPass)
        : m_window(window)
        , m_logicalDevice(logicalDevice)
        , m_renderPass(renderPass)
    {
        //
    }

    VulkanSwapChain::~VulkanSwapChain()
    {
        destroyFrameBuffers();
        destroySwapChain();
    }

    bool VulkanSwapChain::create()
    {
        if (m_swapChain != VK_NULL_HANDLE)
        {
            return true;
        }

        if (!createSwapChain())
        {
            return false;
        }
        if (!createFrameBuffers())
        {
            return false;
        }

        return true;
    }

    bool VulkanSwapChain::recreate()
    {
        destroyFrameBuffers();
        destroySwapChain();

        if (!createSwapChain())
        {
            return false;
        }
        if (!createFrameBuffers())
        {
            return false;
        }

        return true;
    }

    bool VulkanSwapChain::createSwapChain()
    {
        auto context = m_logicalDevice.getContext();
        auto physicalDevice = context->getPickedPhysicalDevice();
        auto surface = context->getVkSurface();

        // select format
        VkSurfaceFormatKHR selFormat = VulkanUtils::findSurfaceFormat(physicalDevice, surface);

        // select present mode
        std::vector<VkPresentModeKHR> presentModes;
        VulkanUtils::getSurfacePresentModes(presentModes, physicalDevice, surface);

        VkPresentModeKHR selPresentMode = VK_PRESENT_MODE_FIFO_KHR;
        // for (const auto& presentMode : presentModes)
        //{
        //    if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        //    {
        //        selPresentMode = presentMode;
        //        break;
        //    }
        //    else if (presentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
        //    {
        //        selPresentMode = presentMode;
        //    }
        //}

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
            const auto& frameBufferSize = m_window.getFrameBufferSize();
            selExtent.width = frameBufferSize.x;
            selExtent.height = frameBufferSize.y;
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
            uint32_t queueFamilyIndies[2] = {static_cast<uint32_t>(graphicsFamilyIndex), static_cast<uint32_t>(presentFamilyIndex)};
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndies;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = selPresentMode;
        createInfo.clipped = VK_TRUE;

        VK_CHECK_RESULT_BOOL(vkCreateSwapchainKHR((VkDevice)m_logicalDevice, &createInfo, nullptr, &m_swapChain))

        std::vector<VkImage> swapChainImages;
        uint32_t swapchainImageCount = 0;
        vkGetSwapchainImagesKHR((VkDevice)m_logicalDevice, m_swapChain, &swapchainImageCount, nullptr);

        swapChainImages.resize(swapchainImageCount);
        vkGetSwapchainImagesKHR((VkDevice)m_logicalDevice, m_swapChain, &swapchainImageCount, swapChainImages.data());
        m_swapChainImageFormat = selFormat.format;
        m_swapChainExtent = selExtent;

        m_images.clear();
        for (auto image : swapChainImages)
        {
            auto vulkanImage = new VulkanImage(m_logicalDevice, image);
            vulkanImage->createImageView(m_swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
            m_images.push_back(vulkanImage);
        }

        return true;
    }

    void VulkanSwapChain::destroySwapChain()
    {
        for (auto image : m_images)
        {
            image->destroyImageView();
            SAFE_DELETE(image);
        }
        m_images.clear();

        vkDestroySwapchainKHR((VkDevice)m_logicalDevice, m_swapChain, nullptr);
    }

    bool VulkanSwapChain::createFrameBuffers()
    {
        m_depthTexture = new VulkanTexture(m_logicalDevice);
        if (!m_depthTexture->createDepthTexture(m_swapChainExtent.width, m_swapChainExtent.height))
        {
            return false;
        }

        std::vector<VulkanImage*> attachments;
        auto count = m_images.size();
        for (size_t i = 0; i < count; ++i)
        {
            attachments.clear();
            attachments.push_back(m_images[i]);
            attachments.push_back(m_depthTexture->getTextureImage());

            auto frameBuffer = new VulkanFrameBuffer(m_logicalDevice, m_renderPass, attachments, {m_swapChainExtent.width, m_swapChainExtent.height});
            if (!frameBuffer->create())
            {
                SAFE_DELETE(frameBuffer);
                return false;
            }

            m_swapChainFrameBuffers.push_back(frameBuffer);
        }

        return true;
    }

    void VulkanSwapChain::destroyFrameBuffers()
    {
        for (auto framebuffer : m_swapChainFrameBuffers)
        {
            SAFE_DELETE(framebuffer);
        }
        m_swapChainFrameBuffers.clear();

        SAFE_DELETE(m_depthTexture);
    }
} // namespace blink
