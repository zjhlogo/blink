/*!
 * \file VulkanDescriptorSets.cpp
 *
 * \author zjhlogo
 * \date 2021/05/18
 *
 *
 */
#include "VulkanDescriptorSets.h"
#include "VulkanBuffer.h"
#include "VulkanDescriptorPool.h"
#include "VulkanImage.h"
#include "VulkanLogicalDevice.h"
#include "VulkanPipeline.h"
#include "VulkanTexture.h"

#include <foundation/Log.h>

#include <array>

NS_BEGIN

VulkanDescriptorSets::VulkanDescriptorSets(VulkanLogicalDevice& logicalDevice, VulkanDescriptorPool& pool, VulkanPipeline& pipeline)
    : m_logicalDevice(logicalDevice)
    , m_descriptorPool(pool)
    , m_pipeline(pipeline)
{
}

VulkanDescriptorSets::~VulkanDescriptorSets()
{
    destroy();
}

bool VulkanDescriptorSets::create(uint32_t count, const std::vector<VulkanBuffer*>& uniformBuffers, VulkanTexture* texture)
{
    std::vector<VkDescriptorSetLayout> layouts(count, m_pipeline.getDestriptorSetLayout());

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.descriptorSetCount = count;
    allocInfo.pSetLayouts = layouts.data();

    m_descriptorSets.resize(count);
    if (vkAllocateDescriptorSets(m_logicalDevice, &allocInfo, m_descriptorSets.data()) != VK_SUCCESS)
    {
        LOGE("allocate descriptor set failed");
        return false;
    }

    for (size_t i = 0; i < count; ++i)
    {
        VkDescriptorBufferInfo bufferInfo;
        bufferInfo.buffer = *(uniformBuffers[i]);
        bufferInfo.offset = 0;
        bufferInfo.range = uniformBuffers[i]->getBufferSize();

        std::array<VkWriteDescriptorSet, 2> descriptorWrites;
        descriptorWrites[0].dstSet = m_descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        VkDescriptorImageInfo imageInfo;
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = texture->getTextureImage()->getImageView();
        imageInfo.sampler = texture->getTextureSampler();

        descriptorWrites[1].dstSet = m_descriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(m_logicalDevice, (uint32_t)descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
    }

    return true;
}

void VulkanDescriptorSets::destroy()
{
    if (!m_descriptorSets.empty())
    {
        vkFreeDescriptorSets(m_logicalDevice, m_descriptorPool, (uint32_t)m_descriptorSets.size(), m_descriptorSets.data());
        m_descriptorSets.clear();
    }
}

NS_END
