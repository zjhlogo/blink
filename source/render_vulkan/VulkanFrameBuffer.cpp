/*********************************************************************
 * \file   VulkanFrameBuffer.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   February 2023
 *********************************************************************/
#include "VulkanFrameBuffer.h"
#include "VulkanImage.h"
#include "VulkanLogicalDevice.h"
#include "VulkanRenderPass.h"

namespace blink
{
    VulkanFrameBuffer::VulkanFrameBuffer(VulkanLogicalDevice& logicalDevice,
                                         VulkanRenderPass& renderPass,
                                         const std::vector<VulkanImage*>& attachments,
                                         const glm::ivec2& frameBufferSize)
        : m_logicalDevice(logicalDevice)
        , m_renderPass(renderPass)
        , m_attachmentImages(attachments)
        , m_frameBufferSize(frameBufferSize)
    {
    }

    VulkanFrameBuffer::~VulkanFrameBuffer()
    {
        destroy();
    }

    bool VulkanFrameBuffer::create()
    {
        if (m_attachmentImages.empty()) return false;

        std::vector<VkImageView> attachments;
        for (auto image : m_attachmentImages)
        {
            attachments.push_back(image->getImageView());
        }

        VkFramebufferCreateInfo frameBufferInfo{};
        frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frameBufferInfo.renderPass = m_renderPass;
        frameBufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        frameBufferInfo.pAttachments = attachments.data();
        frameBufferInfo.width = m_frameBufferSize.x;
        frameBufferInfo.height = m_frameBufferSize.y;
        frameBufferInfo.layers = 1;
        VK_CHECK_RESULT(vkCreateFramebuffer(m_logicalDevice, &frameBufferInfo, nullptr, &m_frameBuffer));

        return true;
    }

    bool VulkanFrameBuffer::recreate()
    {
        destroy();

        return create();
    }

    void VulkanFrameBuffer::destroy()
    {
        vkDestroyFramebuffer(m_logicalDevice, m_frameBuffer, nullptr);
        m_frameBuffer = VK_NULL_HANDLE;
    }
} // namespace blink
