/*!
 * \file VulkanRenderModule.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 *
 */
#pragma once

#include <core/modules/IRenderModule.h>

namespace blink
{
    class VulkanWindow;
    class VulkanContext;
    class VulkanLogicalDevice;
    class VulkanSwapchain;
    class VulkanCommandBuffer;
    class VulkanUniformBuffer;
    class VulkanSemaphore;
    class VulkanFence;

    class VulkanRenderModule final : public IRenderModule
    {
    public:
        bool createDevice(const glm::ivec2& deviceSize) override;
        void destroyDevice() override;

        bool processEvent() override;
        void render() override;

        void waitIdle() override;

        glm::vec2 getSurfaceSize() const override;

        VulkanContext& getContext() const { return *m_context; }
        VulkanLogicalDevice& getLogicalDevice() const { return *m_logicalDevice; }
        VulkanSwapchain& getSwapchain() const { return *m_swapchain; }

        VulkanCommandBuffer& getCommandBuffer() const { return *m_commandBuffer; }
        VulkanUniformBuffer& getPerFrameUniformBuffer() const { return *m_perFrameUniformBuffer; }
        VulkanUniformBuffer& getPerMaterialUniformBuffer() const { return *m_perMaterialUniformBuffer; }

    private:
        bool createSyncObjects();
        void destroySyncObjects();

        bool beginRender();
        void endRender();

        bool beginRenderPass();
        void endRenderPass();

    private:
        VulkanWindow* m_window{};
        VulkanContext* m_context{};
        VulkanLogicalDevice* m_logicalDevice{};
        VulkanFence* m_acquireImageFence{};
        VulkanSwapchain* m_swapchain{};

        VulkanCommandBuffer* m_commandBuffer{};
        VulkanUniformBuffer* m_perFrameUniformBuffer{};
        VulkanUniformBuffer* m_perMaterialUniformBuffer{};
    };
} // namespace blink
