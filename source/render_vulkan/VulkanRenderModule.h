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
    class VulkanRenderPass;
    class VulkanSwapChain;
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

        [[nodiscard]] glm::vec2 getSurfaceSize() const override;

        [[nodiscard]] VulkanContext& getContext() const { return *m_context; }
        [[nodiscard]] VulkanLogicalDevice& getLogicalDevice() const { return *m_logicalDevice; }
        [[nodiscard]] VulkanRenderPass& getRenderPass() const { return *m_renderPass; }
        [[nodiscard]] VulkanSwapChain& getSwapChain() const { return *m_swapChain; }

        [[nodiscard]] VulkanCommandBuffer& getCommandBuffer() const { return *m_commandBuffer; }
        [[nodiscard]] VulkanUniformBuffer& getPerFrameUniformBuffer() const { return *m_perFrameUniformBuffer; }
        [[nodiscard]] VulkanUniformBuffer& getPerMaterialUniformBuffer() const { return *m_perMaterialUniformBuffer; }

    private:
        bool createSyncObjects();
        void destroySyncObjects();

        bool beginRender();
        void endRender();

        void beginRenderPass();
        void endRenderPass();

    private:
        VulkanWindow* m_window{};
        VulkanContext* m_context{};
        VulkanLogicalDevice* m_logicalDevice{};
        VulkanFence* m_acquireImageFence{};
        VulkanRenderPass* m_renderPass{};
        VulkanSwapChain* m_swapChain{};

        VulkanCommandBuffer* m_commandBuffer{};
        VulkanUniformBuffer* m_perFrameUniformBuffer{};
        VulkanUniformBuffer* m_perMaterialUniformBuffer{};
    };
} // namespace blink
