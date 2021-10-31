/*!
 * \file Material.cpp
 *
 * \author zjhlogo
 * \date 2021/10/25
 *
 *
 */
#include "Material.h"

#include <render_vulkan/VulkanBuffer.h>
#include <render_vulkan/VulkanDescriptorSet.h>
#include <render_vulkan/VulkanMemory.h>
#include <render_vulkan/VulkanPipeline.h>
#include <render_vulkan/VulkanSwapchain.h>
#include <render_vulkan/VulkanTexture.h>

NS_BEGIN

Material::Material(VulkanLogicalDevice& logicalDevice, VulkanSwapchain& swapchain, VulkanCommandPool& commandPool, VulkanDescriptorPool& descriptorPool)
    : m_logicalDevice(logicalDevice)
    , m_swapchain(swapchain)
    , m_commandPool(commandPool)
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

    m_uniformBuffer = new VulkanBuffer(m_logicalDevice);
    if (!m_uniformBuffer->createBuffer(sizeof(Uniforms), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE))
    {
        return false;
    }
    m_uniformBuffer->allocateBufferMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    m_texture = new VulkanTexture(m_logicalDevice, m_commandPool);
    if (!m_texture->createTexture2D("resource/viking_room.png")) return false;

    m_descriptorSet = new VulkanDescriptorSet(m_logicalDevice, m_descriptorPool, *m_pipeline);
    if (!m_descriptorSet->create(*m_uniformBuffer, *m_texture)) return false;

    return true;
}

void Material::destroy()
{
    SAFE_DELETE(m_descriptorSet);
    SAFE_DELETE(m_texture);
    SAFE_DELETE(m_uniformBuffer);
    SAFE_DELETE(m_pipeline);
}

void Material::uploadUniformBuffer(const glm::vec3& pos, const glm::quat& rot)
{
    m_uniformBufferObject.localToWorld = glm::translate(glm::identity<glm::mat4>(), pos) * glm::mat4_cast(rot);

    m_uniformBufferObject.worldToCamera = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    const auto& extent = m_swapchain.getImageExtent();
    m_uniformBufferObject.cameraToProjection = glm::perspective(glm::radians(45.0f), extent.width / (float)extent.height, 0.1f, 10.0f);
    m_uniformBufferObject.cameraToProjection[1][1] = -m_uniformBufferObject.cameraToProjection[1][1];

    m_uniformBuffer->getBufferMemory()->uploadData(&m_uniformBufferObject, sizeof(m_uniformBufferObject));
}

NS_END
