/*!
 * \file VulkanBuffer.cpp
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#include "VulkanBuffer.h"
#include "VulkanCommandBuffer.h"
#include "VulkanCommandPool.h"
#include "VulkanContext.h"
#include "VulkanLogicalDevice.h"

#include <foundation/Log.h>

namespace blink
{
    VulkanBuffer::VulkanBuffer(VulkanLogicalDevice& logicalDevice)
        : m_logicalDevice(logicalDevice)
    {
    }

    VulkanBuffer::~VulkanBuffer() { destroyBuffer(); }

    VkBuffer VulkanBuffer::createBuffer(VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkSharingMode mode)
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
            return nullptr;
        }

        m_bufferSize = bufferSize;

        return m_buffer;
    }

    VkBuffer VulkanBuffer::createBufferAndUpload(const void* data,
                                                 VkDeviceSize bufferSize,
                                                 VkBufferUsageFlags usage,
                                                 VkSharingMode mode,
                                                 VulkanCommandPool& pool)
    {
        createBuffer(bufferSize, usage, mode);
        uploadBuffer(data, bufferSize, pool);

        return m_buffer;
    }

    void VulkanBuffer::uploadBuffer(const void* data, VkDeviceSize bufferSize, VulkanCommandPool& pool)
    {
        VulkanBuffer stagingBuffer(m_logicalDevice);
        stagingBuffer.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE);

        auto stagingMem = stagingBuffer.allocateBufferMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        stagingMem->uploadData(data, bufferSize, 0);

        allocateBufferMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        copyBuffer(&stagingBuffer, pool);
    }

    void VulkanBuffer::uploadBuffer(VulkanCommandPool& pool, VulkanMemory::CustomCopyCb cb)
    {
        VulkanBuffer stagingBuffer(m_logicalDevice);
        stagingBuffer.createBuffer(m_bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE);

        auto stagingMem = stagingBuffer.allocateBufferMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        stagingMem->uploadData(0, m_bufferSize, cb);

        allocateBufferMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        copyBuffer(&stagingBuffer, pool);
    }

    void VulkanBuffer::copyBuffer(VulkanBuffer* src, VulkanCommandPool& pool)
    {
        VulkanCommandBuffer commandBuffer(m_logicalDevice, pool);
        commandBuffer.create();
        commandBuffer.beginCommand();

        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = 0;
        copyRegion.size = src->m_bufferSize;
        vkCmdCopyBuffer(commandBuffer, *src, m_buffer, 1, &copyRegion);

        commandBuffer.endCommand();
        commandBuffer.submitCommand();

        commandBuffer.destroy();
    }

    VulkanMemory* VulkanBuffer::allocateBufferMemory(VkMemoryPropertyFlags memProperties)
    {
        if (m_bufferMemory != nullptr)
        {
            return m_bufferMemory;
        }

        VkMemoryRequirements memRequirements{};
        vkGetBufferMemoryRequirements(m_logicalDevice, m_buffer, &memRequirements);

        m_bufferMemory = new VulkanMemory(m_logicalDevice);
        m_bufferMemory->allocateMemory(memProperties, memRequirements);

        if (vkBindBufferMemory(m_logicalDevice, m_buffer, *m_bufferMemory, 0) != VK_SUCCESS)
        {
            LOGE("bind buffer memory failed");
            return nullptr;
        }

        return m_bufferMemory;
    }

    void VulkanBuffer::destroyBufferMemory() { SAFE_DELETE(m_bufferMemory); }

    void VulkanBuffer::destroyBuffer()
    {
        destroyBufferMemory();

        if (m_buffer != nullptr)
        {
            vkDestroyBuffer(m_logicalDevice, m_buffer, nullptr);
            m_buffer = nullptr;
        }
    }

} // namespace blink
