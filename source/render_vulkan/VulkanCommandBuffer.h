/*!
 * \file VulkanCommandBuffer.h
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#pragma once
#include "VulkanBase.h"

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanCommandPool;

    class VulkanCommandBuffer
    {
    public:
        VulkanCommandBuffer(VulkanLogicalDevice& logicalDevice, VulkanCommandPool& commandPool);
        ~VulkanCommandBuffer();

        VulkanCommandBuffer(const VulkanCommandBuffer& buffer) = delete;
        VulkanCommandBuffer(VulkanCommandBuffer&& buffer) = delete;
        VulkanCommandBuffer& operator=(const VulkanCommandBuffer& buffer) = delete;
        VulkanCommandBuffer& operator=(VulkanCommandBuffer&& buffer) = delete;

        bool create();
        void destroy();

        operator VkCommandBuffer() const { return m_commandBuffer; }

        void submitCommand();

        void beginCommand();
        void endCommand();

        void beginRenderPass(VkRenderPass renderPass, VkFramebuffer frameBuffer, const VkRect2D& renderArea);
        void endRenderPass();

    private:
        VulkanLogicalDevice& m_logicalDevice;
        VulkanCommandPool& m_commandPool;
        VkCommandBuffer m_commandBuffer{};
    };
} // namespace blink
