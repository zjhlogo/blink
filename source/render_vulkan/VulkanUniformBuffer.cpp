/**

    @file      VulkanUniformBuffer.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#include "VulkanUniformBuffer.h"
#include "VulkanMemory.h"

#include <render_vulkan/VulkanLogicalDevice.h>

namespace blink
{
    VulkanUniformBuffer::VulkanUniformBuffer(VulkanLogicalDevice& logicalDevice)
        : m_logicalDevice(logicalDevice)
    {
    }

    VulkanUniformBuffer::~VulkanUniformBuffer() { destroy(); }

    VulkanUniformBuffer::operator VkBuffer() const { return *m_gpuBuffer; }

    bool VulkanUniformBuffer::create(VkDeviceSize size)
    {
        destroy();

        m_gpuBuffer = new VulkanBuffer(m_logicalDevice);
        if (!m_gpuBuffer->createBuffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE)) return false;
        if (!m_gpuBuffer->allocateBufferMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) return false;

        m_memBuffer.resize(size);
        m_currentPos = 0;

        return true;
    }

    void VulkanUniformBuffer::destroy()
    {
        m_currentPos = 0;
        m_memBuffer.resize(0);

        SAFE_DELETE(m_gpuBuffer);
    }

    void VulkanUniformBuffer::reset() { m_currentPos = 0; }

    void VulkanUniformBuffer::flushBuffer()
    {
        auto bufferMemory = m_gpuBuffer->getBufferMemory();
        bufferMemory->uploadData(m_memBuffer.data(), m_currentPos, 0);
    }

    bool VulkanUniformBuffer::appendData(const void* data, VkDeviceSize size, VkDescriptorBufferInfo* bufferInfoOut)
    {
        if (size + m_currentPos > m_memBuffer.size()) return false;

        std::memcpy(&m_memBuffer[m_currentPos], data, size);

        if (bufferInfoOut != nullptr)
        {
            bufferInfoOut->buffer = *m_gpuBuffer;
            bufferInfoOut->offset = m_currentPos;
            bufferInfoOut->range = size;
        }

        m_currentPos += size;
        alignBufferOffset();
        return true;
    }

    bool VulkanUniformBuffer::alignBufferOffset()
    {
        auto align = m_logicalDevice.getMinUniformBufferOffsetAlignment();
        auto newPos = ((m_currentPos + align - 1) / align) * align;
        if (newPos >= m_memBuffer.size()) return false;

        m_currentPos = newPos;
        return true;
    }

} // namespace blink
