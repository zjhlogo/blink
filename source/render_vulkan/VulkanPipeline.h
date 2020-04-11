/*!
 * \file VulkanPipeline.h
 *
 * \author zjhlogo
 * \date 2020/02/26
 *
 *
 */
#pragma once
#include <foundation/BaseTypes.h>
#include <foundation/BaseTypesGlm.h>
#include <vulkan/vulkan.hpp>

NS_BEGIN

class VulkanLogicalDevice;

class VulkanPipeline
{
public:
    VulkanPipeline(VulkanLogicalDevice* logicalDevice);
    ~VulkanPipeline();

    bool createRenderPass(const vk::Format& colorAttachmentFormat, const vk::Format& depthAttachmentFormat);
    void destroyRenderPass();

    bool createDescriptorSetLayout();
    void destroyDescriptorSetLayout();

    bool createGraphicsPipeline(uint32_t width, uint32_t height);
    void destroyGraphicsPipeline();

    const vk::RenderPass& getRenderPass() const { return m_renderPass; };
    const vk::DescriptorSetLayout& getDestriptorSetLayout() const { return m_descriptorSetLayout; };
    const vk::PipelineLayout& getPipelineLayout() const { return m_pipelineLayout; };
    const vk::Pipeline& getPipeline() const { return m_pipeline; };

private:
    vk::ShaderModule createShaderModule(const std::vector<uint8>& shaderCode);

private:
    VulkanLogicalDevice* m_logicalDevice{};
    vk::RenderPass m_renderPass;
    vk::DescriptorSetLayout m_descriptorSetLayout;
    vk::PipelineLayout m_pipelineLayout;
    vk::Pipeline m_pipeline;
};

NS_END
