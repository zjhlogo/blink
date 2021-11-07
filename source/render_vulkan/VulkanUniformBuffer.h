/**

    @file      VulkanUniformBuffer.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

#include "VulkanBuffer.h"

#include <vector>

namespace blink
{
    class VulkanUniformBuffer
    {
    public:
        static const VkDeviceSize DEFAULT_UBO_SIZE = 1024 * 1024;

    public:
        VulkanUniformBuffer(VulkanLogicalDevice& logicalDevice);
        ~VulkanUniformBuffer();

        operator VkBuffer() const;

        bool create(VkDeviceSize size = DEFAULT_UBO_SIZE);
        void destroy();
        void reset();
        void flushBuffer();

        size_t getCurrentPos() const { return m_currentPos; };
        bool alignBufferOffset();
        bool appendData(const void* data, VkDeviceSize size);

        const VkDescriptorBufferInfo& getPerFrameBufferInfo() const { return m_perFrameBufferInfo; };
        bool appendPerFrameBufferData(const void* data, VkDeviceSize size);

    private:
        VulkanLogicalDevice& m_logicalDevice;

        VulkanBuffer* m_gpuBuffer{};

        std::vector<uint8_t> m_memBuffer;
        size_t m_currentPos{};

        VkDescriptorBufferInfo m_perFrameBufferInfo{};
    };

} // namespace blink
