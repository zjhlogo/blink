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
#include "VulkanWindow.h"
#include "resources/VulkanTexture.h"
#include "utils/VulkanUtils.h"

#define GLFW_INCLUDE_VULKAN
#include <glfw3/glfw3.h>

namespace blink
{
    VulkanSwapchain::VulkanSwapchain(VulkanWindow& window, VulkanLogicalDevice& logicalDevice)
        : m_window(window)
        , m_logicalDevice(logicalDevice)
    {
        //
    }

    VulkanSwapchain::~VulkanSwapchain()
    {
        //
        destroy();
    }

    bool VulkanSwapchain::create()
    {
        if (!createSwapChain()) return false;
        if (!createSwapchainImageViews()) return false;
        if (!createRenderPass(m_swapChainImageFormat, VulkanUtils::findDepthFormat(m_logicalDevice.getContext()->getPickedPhysicalDevice()))) return false;
        if (!createFrameBuffers(m_renderPass)) return false;

        return true;
    }

    bool VulkanSwapchain::recreate()
    {
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(m_window, &width, &height);

        while (width == 0 || height == 0)
        {
            glfwGetFramebufferSize(m_window, &width, &height);
            glfwWaitEvents();
        }

        m_logicalDevice.waitDeviceIdle();

        destroy();

        if (!createSwapChain()) return false;
        if (!createSwapchainImageViews()) return false;
        if (!createRenderPass(m_swapChainImageFormat, VulkanUtils::findDepthFormat(m_logicalDevice.getContext()->getPickedPhysicalDevice()))) return false;
        if (!createFrameBuffers(m_renderPass)) return false;

        return true;
    }

    void VulkanSwapchain::destroy()
    {
        destroyFrameBuffers();
        destroyRenderPass();
        destroySwapchainImageViews();
        destroySwapChain();
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
            int width, height;
            glfwGetFramebufferSize(*context->getWindow(), &width, &height);
            selExtent.width = width;
            selExtent.height = height;
        }

        // select image count
        uint32_t imageCount = capabilities.minImageCount;
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

        VK_CHECK_RESULT(vkCreateSwapchainKHR(m_logicalDevice, &createInfo, nullptr, &m_swapChain))

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
        for (auto image : m_images)
        {
            image->destroyImage(false);
            SAFE_DELETE(image);
        }
        m_images.clear();

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

    VkRenderPass VulkanSwapchain::createRenderPass(VkFormat colorAttachmentFormat, VkFormat depthAttachmentFormat)
    {
        destroyRenderPass();

        // color attachment
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = colorAttachmentFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        // depth attachment
        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = depthAttachmentFormat;
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        // subPass
        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subPass{};
        subPass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subPass.colorAttachmentCount = 1;
        subPass.pColorAttachments = &colorAttachmentRef;
        subPass.pDepthStencilAttachment = &depthAttachmentRef;

        // create render pass
        VkAttachmentDescription attachments[2] = {colorAttachment, depthAttachment};
        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 2;
        renderPassInfo.pAttachments = attachments;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subPass;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        VK_CHECK_RESULT(vkCreateRenderPass(m_logicalDevice, &renderPassInfo, nullptr, &m_renderPass))
        return m_renderPass;
    }

    void VulkanSwapchain::destroyRenderPass()
    {
        if (m_renderPass != VK_NULL_HANDLE)
        {
            vkDestroyRenderPass(m_logicalDevice, m_renderPass, nullptr);
            m_renderPass = VK_NULL_HANDLE;
        }
    }

    bool VulkanSwapchain::createFrameBuffers(VkRenderPass renderPass)
    {
        SAFE_DELETE(m_depthTexture);

        m_depthTexture = new VulkanTexture(m_logicalDevice);
        if (!m_depthTexture->createDepthTexture(m_swapChainExtent.width, m_swapChainExtent.height)) return false;

        auto count = m_images.size();
        m_swapChainFrameBuffers.resize(count);

        for (size_t i = 0; i < count; ++i)
        {
            auto imageView = m_images[i]->getImageView();

            VkImageView attachments[2] = {imageView, m_depthTexture->getTextureImage()->getImageView()};

            VkFramebufferCreateInfo frameBufferInfo{};
            frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            frameBufferInfo.renderPass = renderPass;
            frameBufferInfo.attachmentCount = 2;
            frameBufferInfo.pAttachments = attachments;
            frameBufferInfo.width = m_swapChainExtent.width;
            frameBufferInfo.height = m_swapChainExtent.height;
            frameBufferInfo.layers = 1;
            VK_CHECK_RESULT(vkCreateFramebuffer(m_logicalDevice, &frameBufferInfo, nullptr, &m_swapChainFrameBuffers[i]))
        }

        return true;
    }

    void VulkanSwapchain::destroyFrameBuffers()
    {
        for (auto framebuffer : m_swapChainFrameBuffers)
        {
            vkDestroyFramebuffer(m_logicalDevice, framebuffer, nullptr);
        }
        m_swapChainFrameBuffers.clear();

        SAFE_DELETE(m_depthTexture);
    }
} // namespace blink
