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

namespace blink
{
    VulkanFence::VulkanFence(VulkanLogicalDevice& logicalDevice)
        : m_logicalDevice(logicalDevice)
    {
        //
    }

    VulkanFence::~VulkanFence()
    {
        //
        destroy();
    }

    bool VulkanFence::create(bool signaled)
    {
        VkFenceCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        if (signaled)
        {
            info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        }

        VK_CHECK_RESULT(vkCreateFence((VkDevice)m_logicalDevice, &info, nullptr, &m_fence))
        return true;
    }

    void VulkanFence::destroy()
    {
        if (m_fence != VK_NULL_HANDLE)
        {
            vkDestroyFence((VkDevice)m_logicalDevice, m_fence, nullptr);
            m_fence = VK_NULL_HANDLE;
        }
    }

    void VulkanFence::wait()
    {
        //
        vkWaitForFences((VkDevice)m_logicalDevice, 1, &m_fence, VK_TRUE, UINT64_MAX);
    }

    void VulkanFence::reset()
    {
        //
        vkResetFences((VkDevice)m_logicalDevice, 1, &m_fence);
    }
} // namespace blink
