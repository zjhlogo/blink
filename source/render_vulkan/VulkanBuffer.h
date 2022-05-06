/*!
 * \file VulkanBuffer.h
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#pragma once
#include "VulkanBase.h"
#include "VulkanMemory.h"

#include <foundation/BaseTypes.h>

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanMemory;

    class VulkanBuffer
    {
    public:
        VulkanBuffer(VulkanLogicalDevice& logicalDevice);
        ~VulkanBuffer();

        operator VkBuffer() const { return m_buffer; };

        VkBuffer createBuffer(VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkSharingMode mode);
        void destroyBuffer();

        VkBuffer createBufferAndUpload(const void* data, VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkSharingMode mode);
        void uploadBuffer(const void* data, VkDeviceSize bufferSize);
        void uploadBuffer(VulkanMemory::CustomCopyCb cb);
        void copyBuffer(VulkanBuffer* src);

        VulkanMemory* allocateBufferMemory(VkMemoryPropertyFlags memProperties);
        void destroyBufferMemory();

        VulkanMemory* getBufferMemory() { return m_bufferMemory; };
        VkDeviceSize getBufferSize() { return m_bufferSize; };

    protected:
        VulkanLogicalDevice& m_logicalDevice;

        VkBuffer m_buffer{};
        VulkanMemory* m_bufferMemory{};
        VkDeviceSize m_bufferSize{};
    };

} // namespace blink
