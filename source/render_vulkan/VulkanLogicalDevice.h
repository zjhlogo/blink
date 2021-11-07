/*!
 * \file VulkanLogicalDevice.h
 *
 * \author zjhlogo
 * \date 2020/04/11
 *
 *
 */
#pragma once
#include <foundation/BaseTypes.h>
#include <vulkan/vulkan.h>

namespace blink
{
    class VulkanContext;

    class VulkanLogicalDevice
    {
    public:
        VulkanLogicalDevice(VulkanContext* context);
        ~VulkanLogicalDevice();

        bool create();
        void destroy();

        operator VkDevice() { return m_logicalDevice; };
        void waitDeviceIdle();
        void waitGraphicsQueueIdle();

        VulkanContext* getContext() { return m_context; };
        VkQueue getGraphicsQueue() { return m_graphicsQueue; };
        VkQueue getPresentQueue() { return m_presentQueue; };
        VkDeviceSize getMinUniformBufferOffsetAlignment() const { return m_minUniformBufferOffsetAlignment; };

    private:
        bool createLogicalDevice();
        void destroyLogicalDevice();

    private:
        VulkanContext* m_context{};

        VkDevice m_logicalDevice{};
        VkQueue m_graphicsQueue{};
        VkQueue m_presentQueue{};
        VkDeviceSize m_minUniformBufferOffsetAlignment{};
    };

} // namespace blink
