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
    vkDestroyCommandPool(m_logicalDevice, m_commandPool, nullptr);
}

VkCommandBuffer VulkanCommandPool::beginSingleTimeCommands()
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = m_commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer{};
    vkAllocateCommandBuffers(m_logicalDevice, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    return commandBuffer;
}

void VulkanCommandPool::endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    auto graphicsQueue = m_logicalDevice.getGraphicsQueue();
    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);

    vkFreeCommandBuffers(m_logicalDevice, m_commandPool, 1, &commandBuffer);
}

NS_END
