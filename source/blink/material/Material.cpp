/**

    @file      Material.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/

#include "Material.h"

#include <render_vulkan/VulkanCommandBuffer.h>
#include <render_vulkan/VulkanDescriptorPool.h>
#include <render_vulkan/VulkanImage.h>
#include <render_vulkan/VulkanLogicalDevice.h>
#include <render_vulkan/VulkanPipeline.h>
#include <render_vulkan/VulkanSwapchain.h>
#include <render_vulkan/VulkanTexture.h>
#include <render_vulkan/VulkanUniformBuffer.h>

NS_BEGIN

Material::Material(VulkanLogicalDevice& logicalDevice, VulkanSwapchain& swapchain, VulkanDescriptorPool& descriptorPool)
    : m_logicalDevice(logicalDevice)
    , m_swapchain(swapchain)
    , m_descriptorPool(descriptorPool)
{
}

Material::~Material()
{
    destroy();
}

bool Material::create()
{
    m_pipeline = new VulkanPipeline(m_logicalDevice, m_swapchain);
    if (!m_pipeline->create()) return false;

    return true;
}

void Material::destroy()
{
    SAFE_DELETE(m_pipeline);
}

void Material::setTexture(VulkanTexture* texture)
{
    m_texture = texture;
}

void Material::bindPipeline(VulkanCommandBuffer& commandBuffer)
{
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *m_pipeline);
}

bool Material::bindUniformBuffer(VulkanCommandBuffer& commandBuffer,
                                 VulkanUniformBuffer& uniformBuffer,
                                 VulkanDescriptorPool& descriptorPool,
                                 const glm::vec3& pos,
                                 const glm::quat& rot)
{
    if (!m_texture) return false;

    glm::mat4 localToWorld = glm::translate(glm::identity<glm::mat4>(), pos) * glm::mat4_cast(rot);

    auto beginOfData = uniformBuffer.getCurrentPos();
    auto dataSize = sizeof(localToWorld);
    if (!uniformBuffer.appendData(&localToWorld, dataSize)) return false;

    // uniforms, textures binding
    auto descriptorSet = descriptorPool.allocateDescriptorSet(m_pipeline->getDestriptorSetLayout());

    VkWriteDescriptorSet descriptorWrites[3]{};

    const auto& perFrameBufferInfo = uniformBuffer.getPerFrameBufferInfo();
    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet = descriptorSet;
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo = &perFrameBufferInfo;

    VkDescriptorBufferInfo perInstanceBufferInfo{};
    perInstanceBufferInfo.buffer = uniformBuffer;
    perInstanceBufferInfo.offset = beginOfData;
    perInstanceBufferInfo.range = dataSize;
    descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[1].dstSet = descriptorSet;
    descriptorWrites[1].dstBinding = 1;
    descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[1].descriptorCount = 1;
    descriptorWrites[1].pBufferInfo = &perInstanceBufferInfo;

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = m_texture->getTextureImage()->getImageView();
    imageInfo.sampler = m_texture->getTextureSampler();
    descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[2].dstSet = descriptorSet;
    descriptorWrites[2].dstBinding = 2;
    descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites[2].descriptorCount = 1;
    descriptorWrites[2].pImageInfo = &imageInfo;

    vkUpdateDescriptorSets(m_logicalDevice, (uint32_t)3, descriptorWrites, 0, nullptr);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline->getPipelineLayout(), 0, 1, &descriptorSet, 0, nullptr);

    return true;
}

NS_END
