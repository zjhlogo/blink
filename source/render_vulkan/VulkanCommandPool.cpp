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
#include "utils/VulkanUtils.h"

NS_BEGIN

VulkanCommandPool::VulkanCommandPool(VulkanContext* context)
{
    int graphicsFamilyIndex{};
    int presentFamilyIndex{};
    VulkanUtils::getBestFitQueueFamilyPropertyIndex(graphicsFamilyIndex,
                                                    presentFamilyIndex,
                                                    context->getPickedPhysicalDevice(),
                                                    context->getVkSurface(),
                                                    context->getQueueFamilyProperties());

    vk::CommandPoolCreateInfo poolInfo;
    poolInfo.queueFamilyIndex = static_cast<uint32_t>(graphicsFamilyIndex);

    m_commandPool = m_logicalDevice.createCommandPool(poolInfo);
}

VulkanCommandPool::~VulkanCommandPool()
{
    m_logicalDevice.destroyCommandPool(m_commandPool);
}

NS_END
