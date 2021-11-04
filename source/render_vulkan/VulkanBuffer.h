/*!
 * \file VulkanBuffer.h
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#pragma once
#include "VulkanMemory.h"

#include <foundation/BaseTypes.h>
#include <vulkan/vulkan.h>

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanCommandPool;
    class VulkanMemory;

    class VulkanBuffer
    {
    public:
        VulkanBuffer(VulkanLogicalDevice& logicalDevice);
        ~VulkanBuffer();

        operator VkBuffer() const { return m_buffer; };

        VkBuffer createBuffer(VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkSharingMode mode);
        void destroyBuffer();

        VkBuffer createBufferAndUpload(const void* data, VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkSharingMode mode, VulkanCommandPool& pool);
        void uploadBuffer(const void* data, VkDeviceSize bufferSize, VulkanCommandPool& pool);
        void uploadBuffer(VulkanCommandPool& pool, VulkanMemory::CustomCopyCb cb);
        void copyBuffer(VulkanBuffer* src, VulkanCommandPool& pool);

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
