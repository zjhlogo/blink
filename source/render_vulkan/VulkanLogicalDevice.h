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

        operator VkDevice() const { return m_logicalDevice; };
        void waitDeviceIdle();
        void waitGraphicsQueueIdle();

        int getGraphicsFamilyIndex() const { return m_graphicsFamilyIndex; };
        int getPresentFamilyIndex() const { return m_presentFamilyIndex; };

        VulkanContext* getContext() const { return m_context; };
        VkQueue getGraphicsQueue() const { return m_graphicsQueue; };
        VkQueue getPresentQueue() const { return m_presentQueue; };
        VkDeviceSize getMinUniformBufferOffsetAlignment() const { return m_minUniformBufferOffsetAlignment; };

    private:
        bool createLogicalDevice();
        void destroyLogicalDevice();

    private:
        VulkanContext* m_context{};

        int m_graphicsFamilyIndex{-1};
        int m_presentFamilyIndex{-1};

        VkDevice m_logicalDevice{};
        VkQueue m_graphicsQueue{};
        VkQueue m_presentQueue{};
        VkDeviceSize m_minUniformBufferOffsetAlignment{};
    };

} // namespace blink
