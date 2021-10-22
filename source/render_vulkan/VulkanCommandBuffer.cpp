/*!
 * \file VulkanCommandBuffer.cpp
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#include "VulkanCommandBuffer.h"
#include "VulkanCommandPool.h"
#include "VulkanContext.h"
#include "VulkanLogicalDevice.h"
#include "utils/VulkanUtils.h"

#include <foundation/Log.h>

#include <array>

NS_BEGIN

VulkanCommandBuffer::VulkanCommandBuffer(VulkanLogicalDevice& logicalDevice, VulkanCommandPool& commandPool)
    : m_logicalDevice(logicalDevice)
    , m_commandPool(commandPool)
{
}

VulkanCommandBuffer::~VulkanCommandBuffer()
{
    destroy();
}

bool VulkanCommandBuffer::create()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = m_commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(m_logicalDevice, &allocInfo, &m_commandBuffer) != VK_SUCCESS)
    {
        LOGE("create command buffer failed");
        return false;
    }

    return true;
}

void VulkanCommandBuffer::destroy()
{
    if (m_commandBuffer != nullptr)
    {
        vkFreeCommandBuffers(m_logicalDevice, m_commandPool, 1, &m_commandBuffer);
        m_commandBuffer = nullptr;
    }
}

void VulkanCommandBuffer::submitCommand()
{
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_commandBuffer;

    auto graphicsQueue = m_logicalDevice.getGraphicsQueue();
    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);
}

void VulkanCommandBuffer::beginCommand()
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

    vkBeginCommandBuffer(m_commandBuffer, &beginInfo);
}

void VulkanCommandBuffer::endCommand()
{
    vkEndCommandBuffer(m_commandBuffer);
}

void VulkanCommandBuffer::beginRenderPass(VkRenderPass renderPass, VkFramebuffer frameBuffer, const VkRect2D& renderArea)
{
    VkRenderPassBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    beginInfo.renderPass = renderPass;
    beginInfo.framebuffer = frameBuffer;
    beginInfo.renderArea = renderArea;

    std::array<VkClearValue, 2> clearValues;
    clearValues[0].color.float32[0] = 0.0f;
    clearValues[0].color.float32[1] = 0.0f;
    clearValues[0].color.float32[2] = 0.0f;
    clearValues[0].color.float32[3] = 1.0f;
    clearValues[1].depthStencil = {1.0f, 0};
    beginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    beginInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(m_commandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanCommandBuffer::endRenderPass()
{
    vkCmdEndRenderPass(m_commandBuffer);
}

NS_END
