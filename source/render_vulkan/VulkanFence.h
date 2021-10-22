/*!
 * \file VulkanFence.h
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#pragma once
#include <foundation/BaseTypes.h>
#include <vulkan/vulkan.h>

NS_BEGIN

class VulkanLogicalDevice;

class VulkanFence
{
public:
    VulkanFence(VulkanLogicalDevice& logicalDevice);
    ~VulkanFence();

    operator VkFence() { return m_fence; };

    bool create(bool signaled = false);
    void destroy();

    void wait();
    void reset();

private:
    VulkanLogicalDevice& m_logicalDevice;

    VkFence m_fence{};
};

NS_END
