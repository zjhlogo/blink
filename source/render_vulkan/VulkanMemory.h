/*!
 * \file VulkanMemory.h
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#pragma once
#include "VulkanBase.h"

#include <functional>

namespace blink
{
    class VulkanLogicalDevice;

    class VulkanMemory
    {
    public:
        typedef std::function<void(void* destBuffer, VkDeviceSize destBufferSize)> CustomCopyCb;

    public:
        VulkanMemory(VulkanLogicalDevice& logicalDevice);
        ~VulkanMemory();

        bool allocateMemory(VkMemoryPropertyFlags memoryProperties, const VkMemoryRequirements& requirement);
        void freeMemory();

        bool uploadData(const void* srcData, VkDeviceSize srcDataSize, VkDeviceSize destOffset);
        bool uploadData(VkDeviceSize destOffset, VkDeviceSize destDataSize, CustomCopyCb cb);

        operator VkDeviceMemory() { return m_memory; };

    private:
        VulkanLogicalDevice& m_logicalDevice;

        VkDeviceMemory m_memory{};
    };

} // namespace blink
