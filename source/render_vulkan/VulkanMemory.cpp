/*!
 * \file VulkanMemory.cpp
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#include "VulkanMemory.h"
#include "VulkanContext.h"
#include "VulkanLogicalDevice.h"

#include <foundation/Log.h>

NS_BEGIN

VulkanMemory::VulkanMemory(VulkanLogicalDevice& logicalDevice)
    : m_logicalDevice(logicalDevice)
{
}

VulkanMemory::~VulkanMemory()
{
    freeMemory();
}

bool VulkanMemory::allocateMemory(VkMemoryPropertyFlags memoryProperties, const VkMemoryRequirements& requirement)
{
    freeMemory();

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = requirement.size;
    allocInfo.memoryTypeIndex = m_logicalDevice.getContext()->findMemoryType(requirement.memoryTypeBits, memoryProperties);

    if (vkAllocateMemory(m_logicalDevice, &allocInfo, nullptr, &m_memory) != VK_SUCCESS)
    {
        LOGE("allocate memory failed");
        return false;
    }

    return true;
}

void VulkanMemory::freeMemory()
{
    if (m_memory != nullptr)
    {
        vkFreeMemory(m_logicalDevice, m_memory, nullptr);
        m_memory = nullptr;
    }
}

bool VulkanMemory::uploadData(void* data, VkDeviceSize size)
{
    void* mapedBuffer{};
    vkMapMemory(m_logicalDevice, m_memory, 0, size, 0, &mapedBuffer);
    memcpy(mapedBuffer, data, static_cast<size_t>(size));
    vkUnmapMemory(m_logicalDevice, m_memory);

    return true;
}

NS_END
