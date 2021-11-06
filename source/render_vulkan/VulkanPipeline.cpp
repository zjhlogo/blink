/*!
 * \file VulkanPipeline.cpp
 *
 * \author zjhlogo
 * \date 2020/02/26
 *
 *
 */
#include "VulkanPipeline.h"
#include "VulkanContext.h"
#include "VulkanDescriptorPool.h"
#include "VulkanImage.h"
#include "VulkanLogicalDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanTexture.h"
#include "utils/VulkanUtils.h"

#include <foundation/File.h>
#include <foundation/Log.h>

namespace blink
{
    VulkanPipeline::VulkanPipeline(VulkanLogicalDevice& logicalDevice, VulkanSwapchain& swapchain, VulkanDescriptorPool& descriptorPool)
        : m_logicalDevice(logicalDevice)
        , m_swapchain(swapchain)
        , m_descriptorPool(descriptorPool)
    {
    }

    VulkanPipeline::~VulkanPipeline() { destroy(); }

    bool VulkanPipeline::create(const std::vector<VkVertexInputBindingDescription>& bindings,
                                const std::vector<VkVertexInputAttributeDescription>& attributes,
                                const tstring& vertexShader,
                                const tstring& fragmentShader,
                                int numTextures,
                                bool wireframe)
    {
        if (!createDescriptorSetLayout(numTextures)) return false;
        if (!createGraphicsPipeline(bindings, attributes, vertexShader, fragmentShader, wireframe)) return false;

        return true;
    }

    void VulkanPipeline::destroy()
    {
        destroyGraphicsPipeline();
        destroyDescriptorSetLayout();
    }

    VkDescriptorSet VulkanPipeline::updateDescriptorSet(const VkDescriptorBufferInfo& pfuBufferInfo,
                                                        const VkDescriptorBufferInfo& piuBufferInfo,
                                                        const std::vector<VkDescriptorImageInfo>& imageInfos)
    {
        // uniforms, textures binding
        auto descriptorSet = m_descriptorPool.allocateDescriptorSet(m_descriptorSetLayout);

        m_writeSets[PerFrameUniformIndex].dstSet = descriptorSet;
        m_writeSets[PerFrameUniformIndex].pBufferInfo = &pfuBufferInfo;

        m_writeSets[PerInstanceUniformIndex].dstSet = descriptorSet;
        m_writeSets[PerInstanceUniformIndex].pBufferInfo = &piuBufferInfo;

        for (int i = 0; i < m_numTextures; ++i)
        {
            m_writeSets[SamplerUniformIndexBegin + i].dstSet = descriptorSet;
            m_writeSets[SamplerUniformIndexBegin + i].pImageInfo = &imageInfos[i];
        }

        vkUpdateDescriptorSets(m_logicalDevice, static_cast<uint32_t>(m_writeSets.size()), m_writeSets.data(), 0, nullptr);

        return descriptorSet;
    }

    VkDescriptorSetLayout VulkanPipeline::createDescriptorSetLayout(int numTextures)
    {
        destroyDescriptorSetLayout();
        m_numTextures = numTextures;

        // pfu
        {
            VkDescriptorSetLayoutBinding pfuLayoutBinding{};
            pfuLayoutBinding.binding = PerFrameUniformIndex;
            pfuLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            pfuLayoutBinding.descriptorCount = 1;
            pfuLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            m_layoutBindings.push_back(pfuLayoutBinding);

            VkWriteDescriptorSet pfuDescriptorWrites{};
            pfuDescriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            pfuDescriptorWrites.dstBinding = PerFrameUniformIndex;
            pfuDescriptorWrites.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            pfuDescriptorWrites.descriptorCount = 1;
            m_writeSets.push_back(pfuDescriptorWrites);
        }

        // piu
        {
            VkDescriptorSetLayoutBinding piuLayoutBinding{};
            piuLayoutBinding.binding = PerInstanceUniformIndex;
            piuLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            piuLayoutBinding.descriptorCount = 1;
            piuLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            m_layoutBindings.push_back(piuLayoutBinding);

            VkWriteDescriptorSet piuDescriptorWrites{};
            piuDescriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            piuDescriptorWrites.dstBinding = PerInstanceUniformIndex;
            piuDescriptorWrites.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            piuDescriptorWrites.descriptorCount = 1;
            m_writeSets.push_back(piuDescriptorWrites);
        }

        // samplers
        for (int i = 0; i < m_numTextures; ++i)
        {
            VkDescriptorSetLayoutBinding samplerLayoutBinding{};
            samplerLayoutBinding.binding = SamplerUniformIndexBegin + i;
            samplerLayoutBinding.descriptorCount = 1;
            samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            m_layoutBindings.push_back(samplerLayoutBinding);

            VkWriteDescriptorSet imageDescriptorWrites{};
            imageDescriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            imageDescriptorWrites.dstBinding = SamplerUniformIndexBegin + i;
            imageDescriptorWrites.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            imageDescriptorWrites.descriptorCount = 1;
            m_writeSets.push_back(imageDescriptorWrites);
        }

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(m_layoutBindings.size());
        layoutInfo.pBindings = m_layoutBindings.data();

        if (vkCreateDescriptorSetLayout(m_logicalDevice, &layoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS)
        {
            LOGE("create descriptor set layout failed");
            return nullptr;
        }

        return m_descriptorSetLayout;
    }

    void VulkanPipeline::destroyDescriptorSetLayout()
    {
        m_numTextures = 0;
        m_layoutBindings.clear();
        m_writeSets.clear();

        if (m_descriptorSetLayout != nullptr)
        {
            vkDestroyDescriptorSetLayout(m_logicalDevice, m_descriptorSetLayout, nullptr);
            m_descriptorSetLayout = nullptr;
        }
    }

    VkPipeline VulkanPipeline::createGraphicsPipeline(const std::vector<VkVertexInputBindingDescription>& bindings,
                                                      const std::vector<VkVertexInputAttributeDescription>& attributes,
                                                      const tstring& vertexShader,
                                                      const tstring& fragmentShader,
                                                      bool wireframe)
    {
        destroyGraphicsPipeline();

        // layout state
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &m_descriptorSetLayout;

        if (vkCreatePipelineLayout(m_logicalDevice, &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
        {
            LOGE("create pipeline layout failed");
            return nullptr;
        }

        VkShaderModule vertShaderModule = createShaderModule(vertexShader);
        VkShaderModule fragShaderModule = createShaderModule(fragmentShader);

        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";

        // shader state
        VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

        // vertex input state
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindings.size());
        vertexInputInfo.pVertexBindingDescriptions = bindings.data();
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributes.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributes.data();

        // input assembly state
        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        // viewport state
        const auto& extent = m_swapchain.getImageExtent();
        VkViewport viewport{0.0f, 0.0f, extent.width * 1.0f, extent.height * 1.0f, 0.0f, 1.0f};
        VkRect2D sissor{{0, 0}, extent};
        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &sissor;

        // rasterizer state
        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = wireframe ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;

        // multisampling state
        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        // depth and stencil state
        VkPipelineDepthStencilStateCreateInfo depthStencil{};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.stencilTestEnable = VK_FALSE;

        // color blending state
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;

        // dynamic state
        VkDynamicState dynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH};
        VkPipelineDynamicStateCreateInfo dynamicState = {};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = 2;
        dynamicState.pDynamicStates = dynamicStates;

        // create pipeline
        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = m_pipelineLayout;
        pipelineInfo.renderPass = m_swapchain.getRenderPass();
        pipelineInfo.subpass = 0;

        auto result = vkCreateGraphicsPipelines(m_logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline);
        vkDestroyShaderModule(m_logicalDevice, fragShaderModule, nullptr);
        vkDestroyShaderModule(m_logicalDevice, vertShaderModule, nullptr);

        if (result != VK_SUCCESS)
        {
            LOGE("create graphics pipeline failed");
            return nullptr;
        }

        return m_pipeline;
    }

    void VulkanPipeline::destroyGraphicsPipeline()
    {
        if (m_pipeline != nullptr)
        {
            vkDestroyPipeline(m_logicalDevice, m_pipeline, nullptr);
            m_pipeline = nullptr;
        }

        if (m_pipelineLayout != nullptr)
        {
            vkDestroyPipelineLayout(m_logicalDevice, m_pipelineLayout, nullptr);
            m_pipelineLayout = nullptr;
        }
    }

    VkShaderModule VulkanPipeline::createShaderModule(const tstring& shaderFile)
    {
        std::vector<uint8> shaderCode;
        if (!File::readFileIntoBuffer(shaderCode, shaderFile))
        {
            return nullptr;
        }

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = shaderCode.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

        VkShaderModule shaderModule{};
        if (vkCreateShaderModule(m_logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            LOGE("create shader module failed");
            return nullptr;
        }

        return shaderModule;
    }

} // namespace blink
