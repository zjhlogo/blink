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
        using CustomCopyCb = std::function<void(void* destBuffer, VkDeviceSize destBufferSize)>;

    public:
        explicit VulkanMemory(VulkanLogicalDevice& logicalDevice);
        ~VulkanMemory();

        VulkanMemory(const VulkanMemory& memory) = delete;
        VulkanMemory(VulkanMemory&& memory) = delete;
        VulkanMemory& operator=(const VulkanMemory& memory) = delete;
        VulkanMemory& operator=(VulkanMemory&& memory) = delete;

        operator VkDeviceMemory() { return m_memory; }

        bool allocateMemory(VkMemoryPropertyFlags memoryProperties, const VkMemoryRequirements& requirement);
        void freeMemory();

        bool uploadData(const void* srcData, VkDeviceSize srcDataSize, VkDeviceSize destOffset);
        bool uploadData(VkDeviceSize destOffset, VkDeviceSize destDataSize, const CustomCopyCb& cb);

    private:
        VulkanLogicalDevice& m_logicalDevice;

        VkDeviceMemory m_memory{};
    };
} // namespace blink
