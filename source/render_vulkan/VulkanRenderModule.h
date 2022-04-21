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
#include <foundation/BaseTypesGlm.h>
#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <glfw3/glfw3.h>

#include <functional>
#include <vector>

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
        VulkanRenderData(VulkanCommandBuffer* _commandBuffer,
                         VulkanUniformBuffer* _pfub,
                         VulkanUniformBuffer* _pmub,
                         VulkanUniformBuffer* _piub)
            : commandBuffer(_commandBuffer)
            , pfub(_pfub)
            , pmub(_pmub)
            , piub(_piub)
        {
        }

        VulkanCommandBuffer* commandBuffer;
        VulkanUniformBuffer* pfub;
        VulkanUniformBuffer* pmub;
        VulkanUniformBuffer* piub;
    };

    class VulkanRenderModule : public IRenderModule
    {
    public:
        VulkanRenderModule() = default;
        virtual ~VulkanRenderModule() = default;

        virtual bool createDevice(const glm::ivec2& deviceSize) override;
        virtual void destroyDevice() override;

        virtual bool processEvent() override;
        virtual RenderResult render(const RenderCb& cb) override;

        virtual void waitIdle() override;

        virtual glm::vec2 getSurfaceSize() const override;

        VulkanContext& getContext() const { return *m_context; };
        VulkanLogicalDevice& getLogicalDevice() const { return *m_logicalDevice; };
        VulkanSwapchain& getSwapchain() const { return *m_swapchain; };

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
