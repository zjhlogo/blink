/*!
 * \file VulkanDescriptorSet.h
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

class VulkanDescriptorSet
{
public:
    VulkanDescriptorSet(VulkanLogicalDevice& logicalDevice, VulkanDescriptorPool& pool, VulkanPipeline& pipeline);
    ~VulkanDescriptorSet();

    operator VkDescriptorSet() const { return m_descriptorSet; };

    bool create(VulkanBuffer& uniformBuffer, VulkanTexture& texture);
    void destroy();

private:
    VulkanLogicalDevice& m_logicalDevice;
    VulkanDescriptorPool& m_descriptorPool;
    VulkanPipeline& m_pipeline;

    VkDescriptorSet m_descriptorSet;
};

NS_END
