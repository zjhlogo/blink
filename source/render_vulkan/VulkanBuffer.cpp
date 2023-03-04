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
#include "VulkanContext.h"
#include "VulkanLogicalDevice.h"

#include <utility>

namespace blink
{
    VulkanBuffer::VulkanBuffer(VulkanLogicalDevice& logicalDevice)
        : m_logicalDevice(logicalDevice)
    {
        //
    }

    VulkanBuffer::~VulkanBuffer()
    {
        //
        destroyBuffer();
    }

    VkBuffer VulkanBuffer::createBuffer(VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkSharingMode mode)
    {
        destroyBuffer();

        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = bufferSize;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = mode;

        VK_CHECK_RESULT_VOID(vkCreateBuffer((VkDevice)m_logicalDevice, &bufferInfo, nullptr, &m_buffer))
        m_bufferSize = bufferSize;
        return m_buffer;
    }

    VkBuffer VulkanBuffer::createBufferAndUpload(const void* data,
                                                 VkDeviceSize bufferSize,
                                                 VkBufferUsageFlags usage,
                                                 VkSharingMode mode)
    {
        createBuffer(bufferSize, usage, mode);
        uploadBuffer(data, bufferSize);

        return m_buffer;
    }

    void VulkanBuffer::uploadBuffer(const void* data, VkDeviceSize bufferSize)
    {
        VulkanBuffer stagingBuffer(m_logicalDevice);
        stagingBuffer.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE);

        auto stagingMem = stagingBuffer.allocateBufferMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
                                                             | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        stagingMem->uploadData(data, bufferSize, 0);

        allocateBufferMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        copyBuffer(&stagingBuffer);
    }

    void VulkanBuffer::uploadBuffer(const VulkanMemory::CustomCopyCb& cb)
    {
        VulkanBuffer stagingBuffer(m_logicalDevice);
        stagingBuffer.createBuffer(m_bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE);

        auto stagingMem = stagingBuffer.allocateBufferMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
                                                             | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        stagingMem->uploadData(0, m_bufferSize, cb);

        allocateBufferMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        copyBuffer(&stagingBuffer);
    }

    void VulkanBuffer::copyBuffer(const VulkanBuffer* src) const
    {
        m_logicalDevice.executeCommand([&](const VulkanCommandBuffer& commandBuffer) {
            VkBufferCopy copyRegion;
            copyRegion.srcOffset = 0;
            copyRegion.dstOffset = 0;
            copyRegion.size = src->m_bufferSize;
            vkCmdCopyBuffer((VkCommandBuffer)commandBuffer, (VkBuffer)*src, m_buffer, 1, &copyRegion);
        });
    }

    VulkanMemory* VulkanBuffer::allocateBufferMemory(VkMemoryPropertyFlags memProperties)
    {
        if (m_bufferMemory != nullptr)
        {
            return m_bufferMemory;
        }

        VkMemoryRequirements memRequirements{};
        vkGetBufferMemoryRequirements((VkDevice)m_logicalDevice, m_buffer, &memRequirements);

        m_bufferMemory = new VulkanMemory(m_logicalDevice);
        m_bufferMemory->allocateMemory(memProperties, memRequirements);

        VK_CHECK_RESULT_VOID(vkBindBufferMemory((VkDevice)m_logicalDevice, m_buffer, (VkDeviceMemory)*m_bufferMemory, 0))
        return m_bufferMemory;
    }

    void VulkanBuffer::destroyBufferMemory()
    {
        //
        SAFE_DELETE(m_bufferMemory);
    }

    void VulkanBuffer::destroyBuffer()
    {
        destroyBufferMemory();

        if (m_buffer != VK_NULL_HANDLE)
        {
            vkDestroyBuffer((VkDevice)m_logicalDevice, m_buffer, nullptr);
            m_buffer = VK_NULL_HANDLE;
        }
    }
} // namespace blink
