/*********************************************************************
 * \file   VulkanFrameBuffer.h
 * \brief
 *
 * \author zjhlogo
 * \date   February 2023
 *********************************************************************/
#pragma once
#include "VulkanBase.h"

#include <foundation/BaseTypesGlm.h>

#include <vector>

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanRenderPass;
    class VulkanImage;

    class VulkanFrameBuffer
    {
    public:
        VulkanFrameBuffer(VulkanLogicalDevice& logicalDevice,
                          VulkanRenderPass& renderPass,
                          const std::vector<VulkanImage*>& attachments,
                          const glm::ivec2& frameBufferSize);
        ~VulkanFrameBuffer();

        bool create();
        bool recreate();
        void destroy();

        operator VkFramebuffer() const { return m_frameBuffer; }

    private:
        VulkanLogicalDevice& m_logicalDevice;
        VulkanRenderPass& m_renderPass;
        std::vector<VulkanImage*> m_attachmentImages;

        glm::ivec2 m_frameBufferSize;
        VkFramebuffer m_frameBuffer{VK_NULL_HANDLE};
    };
} // namespace blink
