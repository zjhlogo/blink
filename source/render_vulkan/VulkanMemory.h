/*!
 * \file VulkanMemory.h
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#pragma once
#include <foundation/BaseTypes.h>
#include <vulkan/vulkan.h>

namespace blink
{
    class VulkanLogicalDevice;

    class VulkanMemory
    {
    public:
        VulkanMemory(VulkanLogicalDevice& logicalDevice);
        ~VulkanMemory();

        bool allocateMemory(VkMemoryPropertyFlags memoryProperties, const VkMemoryRequirements& requirement);
        void freeMemory();

        bool uploadData(const void* data, VkDeviceSize size);

        operator VkDeviceMemory() { return m_memory; };

    private:
        VulkanLogicalDevice& m_logicalDevice;

        VkDeviceMemory m_memory{};
    };

} // namespace blink
