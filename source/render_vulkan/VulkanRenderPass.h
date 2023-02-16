/*********************************************************************
 * \file   VulkanRenderPass.h
 * \brief
 *
 * \author zjhlogo
 * \date   February 2023
 *********************************************************************/
#pragma once
#include "VulkanBase.h"

#include <vector>

namespace blink
{
    class VulkanLogicalDevice;

    class VulkanRenderPass
    {
    public:
        VulkanRenderPass(VulkanLogicalDevice& logicalDevice);
        ~VulkanRenderPass();

        VulkanRenderPass(const VulkanRenderPass&) = delete;
        VulkanRenderPass(VulkanRenderPass&&) = delete;
        VulkanRenderPass& operator=(const VulkanRenderPass&) = delete;
        VulkanRenderPass& operator=(VulkanRenderPass&&) = delete;

        operator VkRenderPass() const { return m_renderPass; }

        bool create();

    private:
        VkRenderPass createRenderPass(VkFormat colorAttachmentFormat, VkFormat depthAttachmentFormat);
        void destroyRenderPass();

    private:
        VulkanLogicalDevice& m_logicalDevice;
        VkRenderPass m_renderPass{};
    };
} // namespace blink
