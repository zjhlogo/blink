/*!
 * \file VulkanBuffer.cpp
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#include "VulkanBuffer.h"
#include "VulkanContext.h"
#include "VulkanLogicalDevice.h"
#include "VulkanMemory.h"

#include <foundation/Log.h>

NS_BEGIN

VulkanBuffer::VulkanBuffer(VulkanLogicalDevice& logicalDevice)
    : m_logicalDevice(logicalDevice)
{
}

VulkanBuffer::~VulkanBuffer()
{
    destroyBuffer();
}

bool VulkanBuffer::createBuffer(VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkSharingMode mode)
{
    destroyBuffer();

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = bufferSize;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = mode;

    if (vkCreateBuffer(m_logicalDevice, &bufferInfo, nullptr, &m_buffer) != VK_SUCCESS)
    {
        LOGE("create buffer failed");
        return false;
    }

    return true;
}

bool VulkanBuffer::allocateBufferMemory(VkMemoryPropertyFlags memProperties)
{
    if (m_bufferMemory != nullptr)
    {
        LOGE("can not allocate buffer memory again, destroy its memory first");
        return false;
    }

    VkMemoryRequirements memRequirements{};
    vkGetBufferMemoryRequirements(m_logicalDevice, m_buffer, &memRequirements);

    m_bufferMemory = new VulkanMemory(m_logicalDevice);
    m_bufferMemory->allocateMemory(memProperties, memRequirements);

    if (vkBindBufferMemory(m_logicalDevice, m_buffer, *m_bufferMemory, 0) != VK_SUCCESS)
    {
        LOGE("bind buffer memory failed");
        return false;
    }

    return true;
}

void VulkanBuffer::destroyBufferMemory()
{
    SAFE_DELETE(m_bufferMemory);
}

void VulkanBuffer::destroyBuffer()
{
    destroyBufferMemory();

    if (m_buffer != nullptr)
    {
        vkDestroyBuffer(m_logicalDevice, m_buffer, nullptr);
        m_buffer = nullptr;
    }
}

NS_END
