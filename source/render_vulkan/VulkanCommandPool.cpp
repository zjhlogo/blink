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

#include <foundation/Log.h>

NS_BEGIN

VulkanCommandPool::VulkanCommandPool(VulkanLogicalDevice& logicalDevice)
    : m_logicalDevice(logicalDevice)
{
}

VulkanCommandPool::~VulkanCommandPool()
{
    destroy();
}

bool VulkanCommandPool::create()
{
    int graphicsFamilyIndex{};
    int presentFamilyIndex{};

    auto context = m_logicalDevice.getContext();
    VulkanUtils::getBestFitQueueFamilyPropertyIndex(graphicsFamilyIndex,
                                                    presentFamilyIndex,
                                                    context->getPickedPhysicalDevice(),
                                                    context->getVkSurface(),
                                                    context->getQueueFamilyProperties());

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = static_cast<uint32_t>(graphicsFamilyIndex);

    if (vkCreateCommandPool(m_logicalDevice, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
    {
        LOGE("create command pool failed");
        return false;
    }

    return true;
}

void VulkanCommandPool::destroy()
{
    if (m_commandPool != nullptr)
    {
        vkDestroyCommandPool(m_logicalDevice, m_commandPool, nullptr);
        m_commandPool = nullptr;
    }
}

NS_END
