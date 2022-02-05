/*!
 * \file VulkanDescriptorPool.cpp
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#include "VulkanDescriptorPool.h"
#include "VulkanLogicalDevice.h"

#include <foundation/Log.h>

#include <array>

namespace blink
{
    VulkanDescriptorPool::VulkanDescriptorPool(VulkanLogicalDevice& logicalDevice)
        : m_logicalDevice(logicalDevice)
    {
        //
    }

    VulkanDescriptorPool::~VulkanDescriptorPool()
    {
        //
        destroy();
    }

    bool VulkanDescriptorPool::create(uint32_t count)
    {
        std::array<VkDescriptorPoolSize, 2> poolSizes;
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = count * 2;
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = count;

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        // poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();

        poolInfo.maxSets = count;

        if (vkCreateDescriptorPool(m_logicalDevice, &poolInfo, nullptr, &m_descriptorPool) != VK_SUCCESS)
        {
            LOGE("create descriptor pool failed");
            return false;
        }

        return true;
    }

    void VulkanDescriptorPool::destroy()
    {
        reset();

        if (m_descriptorPool != nullptr)
        {
            vkDestroyDescriptorPool(m_logicalDevice, m_descriptorPool, nullptr);
            m_descriptorPool = nullptr;
        }
    }

    void VulkanDescriptorPool::reset()
    {
        if (m_descriptorPool != nullptr)
        {
            vkResetDescriptorPool(m_logicalDevice, m_descriptorPool, 0);
        }
    }

    VkDescriptorSet VulkanDescriptorPool::allocateDescriptorSet(VkDescriptorSetLayout layout)
    {
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = m_descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &layout;

        VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
        if (vkAllocateDescriptorSets(m_logicalDevice, &allocInfo, &descriptorSet) != VK_SUCCESS)
        {
            LOGE("allocate descriptor set failed");
        }

        return descriptorSet;
    }

} // namespace blink
