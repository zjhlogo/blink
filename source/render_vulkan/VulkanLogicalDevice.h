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

#include <functional>

namespace blink
{
    class VulkanContext;
    class VulkanCommandPool;
    class VulkanDescriptorPool;
    class VulkanCommandBuffer;

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

        void resetDescriptorPool();
        void executeCommand(std::function<void(VulkanCommandBuffer& commandBuffer)>&& cb);

        int getGraphicsFamilyIndex() const { return m_graphicsFamilyIndex; };
        int getPresentFamilyIndex() const { return m_presentFamilyIndex; };

        VulkanContext* getContext() const { return m_context; };
        VkQueue getGraphicsQueue() const { return m_graphicsQueue; };
        VkQueue getPresentQueue() const { return m_presentQueue; };
        VulkanCommandPool& getCommandPool() const { return *m_commandPool; };
        VulkanDescriptorPool& getDescriptorPool() const { return *m_descriptorPool; };

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
        VulkanCommandPool* m_commandPool{};
        VulkanDescriptorPool* m_descriptorPool{};

        VkDeviceSize m_minUniformBufferOffsetAlignment{};
    };

} // namespace blink
