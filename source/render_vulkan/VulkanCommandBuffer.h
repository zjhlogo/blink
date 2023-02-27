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

        VulkanCommandBuffer(const VulkanCommandBuffer&) = delete;
        VulkanCommandBuffer(VulkanCommandBuffer&&) = delete;
        VulkanCommandBuffer& operator=(const VulkanCommandBuffer&) = delete;
        VulkanCommandBuffer& operator=(VulkanCommandBuffer&&) = delete;

        bool create();
        void destroy();

        explicit operator VkCommandBuffer() const { return m_commandBuffer; }

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
