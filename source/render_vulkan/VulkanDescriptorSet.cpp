/*!
 * \file VulkanDescriptorSet.cpp
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#include "VulkanDescriptorSet.h"
#include "VulkanBuffer.h"
#include "VulkanDescriptorPool.h"
#include "VulkanImage.h"
#include "VulkanLogicalDevice.h"
#include "VulkanPipeline.h"
#include "VulkanTexture.h"

#include <foundation/Log.h>

#include <array>

NS_BEGIN

VulkanDescriptorSet::VulkanDescriptorSet(VulkanLogicalDevice& logicalDevice, VulkanDescriptorPool& pool, VulkanPipeline& pipeline)
    : m_logicalDevice(logicalDevice)
    , m_descriptorPool(pool)
    , m_pipeline(pipeline)
{
}

VulkanDescriptorSet::~VulkanDescriptorSet()
{
    destroy();
}

bool VulkanDescriptorSet::create(VulkanBuffer& uniformBuffer, VulkanTexture& texture)
{
    VkDescriptorSetLayout layout = m_pipeline.getDestriptorSetLayout();

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &layout;

    if (vkAllocateDescriptorSets(m_logicalDevice, &allocInfo, &m_descriptorSet) != VK_SUCCESS)
    {
        LOGE("allocate descriptor set failed");
        return false;
    }

    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = uniformBuffer;
    bufferInfo.offset = 0;
    bufferInfo.range = uniformBuffer.getBufferSize();

    VkWriteDescriptorSet descriptorWrites[2]{};
    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet = m_descriptorSet;
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo = &bufferInfo;

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = texture.getTextureImage()->getImageView();
    imageInfo.sampler = texture.getTextureSampler();

    descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[1].dstSet = m_descriptorSet;
    descriptorWrites[1].dstBinding = 1;
    descriptorWrites[1].dstArrayElement = 0;
    descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites[1].descriptorCount = 1;
    descriptorWrites[1].pImageInfo = &imageInfo;

    vkUpdateDescriptorSets(m_logicalDevice, (uint32_t)2, descriptorWrites, 0, nullptr);

    return true;
}

void VulkanDescriptorSet::destroy()
{
    if (m_descriptorSet != nullptr)
    {
        vkFreeDescriptorSets(m_logicalDevice, m_descriptorPool, 1, &m_descriptorSet);
        m_descriptorSet = nullptr;
    }
}

NS_END
