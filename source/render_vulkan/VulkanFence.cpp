/*!
 * \file VulkanFence.cpp
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#include "VulkanFence.h"
#include "VulkanLogicalDevice.h"

#include <foundation/Log.h>

NS_BEGIN

VulkanFence::VulkanFence(VulkanLogicalDevice& logicalDevice)
    : m_logicalDevice(logicalDevice)
{
}

VulkanFence::~VulkanFence()
{
    destroy();
}

bool VulkanFence::create(bool signaled)
{
    VkFenceCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    if (signaled) info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (vkCreateFence(m_logicalDevice, &info, nullptr, &m_fence) != VK_SUCCESS)
    {
        LOGE("create fence failed");
        return false;
    }

    return true;
}

void VulkanFence::destroy()
{
    if (m_fence != nullptr)
    {
        vkDestroyFence(m_logicalDevice, m_fence, nullptr);
        m_fence = nullptr;
    }
}

void VulkanFence::wait()
{
    vkWaitForFences(m_logicalDevice, 1, &m_fence, VK_TRUE, UINT64_MAX);
}

void VulkanFence::reset()
{
    vkResetFences(m_logicalDevice, 1, &m_fence);
}

NS_END
