/*********************************************************************
 * \file   VulkanRenderPass.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   February 2023
 *********************************************************************/
#include "VulkanRenderPass.h"
#include "VulkanContext.h"
#include "VulkanImage.h"
#include "VulkanLogicalDevice.h"
#include "VulkanWindow.h"
#include "utils/VulkanUtils.h"

namespace blink
{
    VulkanRenderPass::VulkanRenderPass(VulkanLogicalDevice& logicalDevice)
        : m_logicalDevice(logicalDevice)
    {
        //
    }

    VulkanRenderPass::~VulkanRenderPass()
    {
        //
        destroyRenderPass();
    }

    bool VulkanRenderPass::create()
    {
        auto context = m_logicalDevice.getContext();
        auto physicalDevice = context->getPickedPhysicalDevice();
        auto surface = context->getVkSurface();
        VkSurfaceFormatKHR selFormat = VulkanUtils::findSurfaceFormat(physicalDevice, surface);
        auto depthFormat = VulkanUtils::findDepthFormat(physicalDevice);

        return createRenderPass(selFormat.format, depthFormat);
    }

    VkRenderPass VulkanRenderPass::createRenderPass(VkFormat colorAttachmentFormat, VkFormat depthAttachmentFormat)
    {
        if (m_renderPass != VK_NULL_HANDLE)
        {
            return m_renderPass;
        }

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

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        // create render pass
        VkAttachmentDescription attachments[2] = {colorAttachment, depthAttachment};
        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 2;
        renderPassInfo.pAttachments = attachments;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subPass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        VK_CHECK_RESULT_VOID(vkCreateRenderPass((VkDevice)m_logicalDevice, &renderPassInfo, nullptr, &m_renderPass))
        return m_renderPass;
    }

    void VulkanRenderPass::destroyRenderPass()
    {
        if (m_renderPass != VK_NULL_HANDLE)
        {
            vkDestroyRenderPass((VkDevice)m_logicalDevice, m_renderPass, nullptr);
            m_renderPass = VK_NULL_HANDLE;
        }
    }
} // namespace blink
