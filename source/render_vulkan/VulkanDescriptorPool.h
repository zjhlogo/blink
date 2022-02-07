/*!
 * \file VulkanDescriptorPool.h
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

    class VulkanDescriptorPool
    {
    public:
        static const uint32_t DEFAULT_DESCRIPTOR_COUNT = 1024;

    public:
        VulkanDescriptorPool(VulkanLogicalDevice& logicalDevice);
        ~VulkanDescriptorPool();

        operator VkDescriptorPool() const { return m_descriptorPool; };

        bool create(uint32_t count = DEFAULT_DESCRIPTOR_COUNT);
        void destroy();
        void reset();

        VkDescriptorSet allocateDescriptorSet(VkDescriptorSetLayout layout);

    private:
        VulkanLogicalDevice& m_logicalDevice;
        VkDescriptorPool m_descriptorPool{};
    };

} // namespace blink
