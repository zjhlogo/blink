/*!
 * \file VulkanCommandPool.cpp
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#include "VulkanCommandPool.h"
#include "VulkanContext.h"
#include "VulkanLogicalDevice.h"
#include "utils/VulkanUtils.h"

namespace blink
{
    VulkanCommandPool::VulkanCommandPool(VulkanLogicalDevice& logicalDevice)
        : m_logicalDevice(logicalDevice)
    {
        //
    }

    VulkanCommandPool::~VulkanCommandPool()
    {
        //
        destroy();
    }

    bool VulkanCommandPool::create()
    {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = static_cast<uint32_t>(m_logicalDevice.getGraphicsFamilyIndex());

        VK_CHECK_RESULT(vkCreateCommandPool(m_logicalDevice, &poolInfo, nullptr, &m_commandPool))
        return true;
    }

    void VulkanCommandPool::destroy()
    {
        if (m_commandPool != VK_NULL_HANDLE)
        {
            vkDestroyCommandPool(m_logicalDevice, m_commandPool, nullptr);
            m_commandPool = VK_NULL_HANDLE;
        }
    }
} // namespace blink
