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

class VulkanPipeline
{
public:
    VulkanPipeline(const vk::Device& logicalDevice);
    ~VulkanPipeline();

    bool createRenderPass(const vk::Format& colorAttachmentFormat, const vk::Format& depthAttachmentFormat);
    void destroyRenderPass();

    bool createDescriptorSetLayout();
    void destroyDescriptorSetLayout();

    bool createGraphicsPipeline(float width, float height);
    void destroyGraphicsPipeline();

private:
    vk::ShaderModule createShaderModule(const std::vector<uint8>& shaderCode);

private:
    vk::Device m_logicalDevice;
    vk::RenderPass m_renderPass;
    vk::DescriptorSetLayout m_descriptorSetLayout;
    vk::PipelineLayout m_pipelineLayout;
    vk::Pipeline m_pipeline;

};

NS_END
