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

namespace blink
{
    VulkanMemory::VulkanMemory(VulkanLogicalDevice& logicalDevice)
        : m_logicalDevice(logicalDevice)
    {
        //
    }

    VulkanMemory::~VulkanMemory()
    {
        //
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

    bool VulkanMemory::uploadData(const void* srcData, VkDeviceSize srcDataSize, VkDeviceSize destOffset)
    {
        void* mapedBuffer{};
        vkMapMemory(m_logicalDevice, m_memory, destOffset, srcDataSize, 0, &mapedBuffer);
        memcpy(mapedBuffer, srcData, static_cast<size_t>(srcDataSize));
        vkUnmapMemory(m_logicalDevice, m_memory);

        return true;
    }

    bool VulkanMemory::uploadData(VkDeviceSize destOffset, VkDeviceSize destDataSize, CustomCopyCb cb)
    {
        void* mapedBuffer{};
        vkMapMemory(m_logicalDevice, m_memory, destOffset, destDataSize, 0, &mapedBuffer);
        cb(mapedBuffer, destDataSize);
        vkUnmapMemory(m_logicalDevice, m_memory);

        return true;
    }

} // namespace blink
