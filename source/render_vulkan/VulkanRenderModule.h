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

    class VulkanRenderData : public IRenderData
    {
    public:
        VulkanRenderData(VulkanCommandBuffer* commandBuffer,
                         VulkanUniformBuffer* fub,
                         VulkanUniformBuffer* mub,
                         VulkanUniformBuffer* eub)
            : commandBuffer(commandBuffer)
            , fub(fub)
            , mub(mub)
            , eub(eub) { }

        VulkanCommandBuffer* commandBuffer;
        VulkanUniformBuffer* fub;
        VulkanUniformBuffer* mub;
        VulkanUniformBuffer* eub;
    };

    class VulkanRenderModule final : public IRenderModule
    {
    public:
        bool createDevice(const glm::ivec2& deviceSize) override;
        void destroyDevice() override;

        bool processEvent() override;
        RenderResult render(const RenderCb& cb) override;

        void waitIdle() override;

        glm::vec2 getSurfaceSize() const override;

        VulkanContext& getContext() const { return *m_context; }
        VulkanLogicalDevice& getLogicalDevice() const { return *m_logicalDevice; }
        VulkanSwapchain& getSwapchain() const { return *m_swapchain; }

    private:
        bool createSyncObjects();
        void destroySyncObjects();

    private:
        VulkanFence* m_acquireImageFence{};

        VulkanWindow* m_window{};
        VulkanContext* m_context{};
        VulkanLogicalDevice* m_logicalDevice{};
        VulkanSwapchain* m_swapchain{};
        VulkanCommandBuffer* m_commandBuffer{};

        VulkanUniformBuffer* m_perFrameUniformBuffer{};
        VulkanUniformBuffer* m_perMaterialUniformBuffer{};
        VulkanUniformBuffer* m_perInstanceUniformBuffer{};
    };
} // namespace blink
