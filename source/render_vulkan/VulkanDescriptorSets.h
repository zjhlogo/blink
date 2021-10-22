/*!
 * \file VulkanDescriptorSets.h
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#pragma once
#include <foundation/BaseTypes.h>
#include <vulkan/vulkan.h>

#include <vector>
NS_BEGIN

class VulkanLogicalDevice;
class VulkanDescriptorPool;
class VulkanPipeline;
class VulkanBuffer;
class VulkanTexture;

class VulkanDescriptorSets
{
public:
    VulkanDescriptorSets(VulkanLogicalDevice& logicalDevice, VulkanDescriptorPool& pool, VulkanPipeline& pipeline);
    ~VulkanDescriptorSets();

    VkDescriptorSet getDescriptorSet(size_t index) const { return m_descriptorSets[index]; };

    bool create(uint32_t count, const std::vector<VulkanBuffer*>& uniformBuffers, VulkanTexture* texture);
    void destroy();

private:
    VulkanLogicalDevice& m_logicalDevice;
    VulkanDescriptorPool& m_descriptorPool;
    VulkanPipeline& m_pipeline;

    std::vector<VkDescriptorSet> m_descriptorSets;
};

NS_END
