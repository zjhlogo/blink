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

class VulkanContext;
class VulkanLogicalDevice;
class VulkanSwapchain;

class VulkanPipeline
{
public:
    VulkanPipeline();
    ~VulkanPipeline();

    bool initialize(VulkanContext* context, VulkanLogicalDevice* logicalDevice, VulkanSwapchain* swapchain);
    void terminate();

    const vk::RenderPass& getRenderPass() const { return m_renderPass; };
    const vk::DescriptorSetLayout& getDestriptorSetLayout() const { return m_descriptorSetLayout; };
    const vk::PipelineLayout& getPipelineLayout() const { return m_pipelineLayout; };
    const vk::Pipeline& getPipeline() const { return m_pipeline; };

private:
    bool createRenderPass(const vk::Format& colorAttachmentFormat, const vk::Format& depthAttachmentFormat);
    void destroyRenderPass();

    bool createDescriptorSetLayout();
    void destroyDescriptorSetLayout();

    bool createGraphicsPipeline(uint32_t width, uint32_t height);
    void destroyGraphicsPipeline();

    vk::ShaderModule createShaderModule(const std::vector<uint8>& shaderCode);

private:
    VulkanContext* m_context{};
    VulkanLogicalDevice* m_logicalDevice{};
    VulkanSwapchain* m_swapchain{};

    vk::RenderPass m_renderPass;
    vk::DescriptorSetLayout m_descriptorSetLayout;
    vk::PipelineLayout m_pipelineLayout;
    vk::Pipeline m_pipeline;
};

NS_END
