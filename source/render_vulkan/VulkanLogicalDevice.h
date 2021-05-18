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

NS_BEGIN

class VulkanContext;

class VulkanLogicalDevice
{
public:
    VulkanLogicalDevice(VulkanContext* context);
    ~VulkanLogicalDevice();

    bool create();
    void destroy();

    operator VkDevice() { return m_logicalDevice; };

    VulkanContext* getContext() { return m_context; };
    VkQueue getGraphicsQueue() { return m_graphicsQueue; };
    VkQueue getPresentQueue() { return m_presentQueue; };

private:
    bool createLogicalDevice();
    void destroyLogicalDevice();

private:
    VulkanContext* m_context{};

    VkDevice m_logicalDevice{};
    VkQueue m_graphicsQueue{};
    VkQueue m_presentQueue{};
};

NS_END
