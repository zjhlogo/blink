/*!
 * \file VulkanBuffer.h
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 * 
 */
#pragma once
#include <foundation/BaseTypes.h>
#include <vulkan/vulkan.h>

NS_BEGIN

class VulkanLogicalDevice;
class VulkanMemory;

class VulkanBuffer
{
public:
    VulkanBuffer(VulkanLogicalDevice& logicalDevice);
    ~VulkanBuffer();

    bool createBuffer(VkDeviceSize bufferSize, VkBufferUsageFlags usage, VkSharingMode mode);
    void destroyBuffer();

    bool allocateBufferMemory(VkMemoryPropertyFlags memProperties);
    void destroyBufferMemory();

    VulkanMemory* getBufferMemory() { return m_bufferMemory; };

private:
    VulkanLogicalDevice& m_logicalDevice;

    VkBuffer m_buffer{};
    VulkanMemory* m_bufferMemory{};

};

NS_END
