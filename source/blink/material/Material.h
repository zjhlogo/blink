/*!
 * \file Material.h
 *
 * \author zjhlogo
 * \date 2021/10/25
 *
 *
 */
#pragma once
#include <foundation/BaseTypesGlm.h>

#include <vector>

NS_BEGIN

class VulkanLogicalDevice;
class VulkanSwapchain;
class VulkanPipeline;
class VulkanCommandPool;
class VulkanDescriptorPool;
class VulkanTexture;
class VulkanBuffer;
class VulkanDescriptorSet;

class Material
{
private:
    struct Uniforms
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

public:
    Material(VulkanLogicalDevice& logicalDevice, VulkanSwapchain& swapchain, VulkanCommandPool& commandPool, VulkanDescriptorPool& descriptorPool);
    ~Material();

    bool create();
    void destroy();

    VulkanBuffer& getUniformBuffer() const { return *m_uniformBuffer; };
    VulkanDescriptorSet& getDescriptorSet() const { return *m_descriptorSet; };
    VulkanPipeline& getPipeline() { return *m_pipeline; };

    void updateUniformBuffer();

private:
    VulkanLogicalDevice& m_logicalDevice;
    VulkanSwapchain& m_swapchain;
    VulkanCommandPool& m_commandPool;
    VulkanDescriptorPool& m_descriptorPool;

    VulkanPipeline* m_pipeline{};
    VulkanBuffer* m_uniformBuffer{};
    VulkanDescriptorSet* m_descriptorSet{};

    VulkanTexture* m_texture{};

    Uniforms m_uniformBufferObject{};
};

NS_END
