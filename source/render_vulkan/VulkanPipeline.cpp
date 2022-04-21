/*!
 * \file VulkanPipeline.cpp
 *
 * \author zjhlogo
 * \date 2020/02/26
 *
 *
 */
#include "VulkanPipeline.h"
#include "VulkanCommandBuffer.h"
#include "VulkanContext.h"
#include "VulkanDescriptorPool.h"
#include "VulkanImage.h"
#include "VulkanLogicalDevice.h"
#include "VulkanSwapchain.h"
#include "utils/VulkanUtils.h"

#include <foundation/File.h>
#include <foundation/Log.h>
#include <spirv-cross/spirv_glsl.hpp>

namespace blink
{
    VulkanPipeline::VulkanPipeline(VulkanLogicalDevice& logicalDevice, VulkanSwapchain& swapchain)
        : m_logicalDevice(logicalDevice)
        , m_swapchain(swapchain)
    {
        //
    }

    VulkanPipeline::~VulkanPipeline()
    {
        //
        destroy();
    }

    bool VulkanPipeline::create(const tstring& vertexShader,
                                const tstring& fragmentShader,
                                VkPolygonMode polygonMode,
                                PrimitiveTopology topology)
    {
        m_vertexShader = vertexShader;
        m_fragmentShader = fragmentShader;
        m_polygonMode = polygonMode;

        if (topology == PrimitiveTopology::LineList)
        {
            m_topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
        }
        else if (topology == PrimitiveTopology::TriangleList)
        {
            m_topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        }

        return recreate();
    }

    bool VulkanPipeline::recreate()
    {
        destroy();

        std::vector<uint8> vertexShaderCode;
        if (!File::readFileIntoBuffer(vertexShaderCode, m_vertexShader)) return false;

        std::vector<uint8> fragmentShaderCode;
        if (!File::readFileIntoBuffer(fragmentShaderCode, m_fragmentShader)) return false;

        // generate vertex input desc
        std::vector<VkVertexInputBindingDescription> vertexInputBindings;
        std::vector<VkVertexInputAttributeDescription> vertexInputAttributes;
        m_vertexInputMasks = generateVertexInputDesc(vertexInputBindings, vertexInputAttributes, vertexShaderCode);

        // generate layout bindings
        std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
        m_numTextures = (int)generateDescriptorSetLayout(layoutBindings, m_writeSets, fragmentShaderCode);

        if (createDescriptorSetLayout(layoutBindings) == VK_NULL_HANDLE) return false;
        if (createGraphicsPipeline(vertexShaderCode,
                                   fragmentShaderCode,
                                   vertexInputBindings,
                                   vertexInputAttributes,
                                   m_polygonMode,
                                   m_topology)
            == VK_NULL_HANDLE)
            return false;

        return true;
    }

    void VulkanPipeline::destroy()
    {
        destroyGraphicsPipeline();
        destroyDescriptorSetLayout();

        m_numTextures = 0;
        m_writeSets.clear();
        m_vertexInputMasks = 0;
    }

    bool VulkanPipeline::bindDescriptorSets(VulkanCommandBuffer& commandBuffer,
                                            const VkDescriptorBufferInfo& pfuBufferInfo,
                                            const VkDescriptorBufferInfo& pmuBufferInfo,
                                            const VkDescriptorBufferInfo& piuBufferInfo,
                                            const std::vector<VkDescriptorImageInfo>& imageInfos)
    {
        // uniforms binding
        auto descriptorSet = m_logicalDevice.getDescriptorPool().allocateDescriptorSet(m_descriptorSetLayout);

        m_writeSets[PerFrameUniformIndex].dstSet = descriptorSet;
        m_writeSets[PerFrameUniformIndex].pBufferInfo = &pfuBufferInfo;

        m_writeSets[PerMaterialUniformIndex].dstSet = descriptorSet;
        m_writeSets[PerMaterialUniformIndex].pBufferInfo = &pmuBufferInfo;

        m_writeSets[PerInstanceUniformIndex].dstSet = descriptorSet;
        m_writeSets[PerInstanceUniformIndex].pBufferInfo = &piuBufferInfo;

        // sampler binding
        for (int i = 0; i < m_numTextures; ++i)
        {
            m_writeSets[SamplerUniformIndexBegin + i].dstSet = descriptorSet;
            m_writeSets[SamplerUniformIndexBegin + i].pImageInfo = &imageInfos[i];
        }

        vkUpdateDescriptorSets(m_logicalDevice, static_cast<uint32_t>(m_writeSets.size()), m_writeSets.data(), 0, nullptr);

        vkCmdBindDescriptorSets(commandBuffer,
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                m_pipelineLayout,
                                0,
                                1,
                                &descriptorSet,
                                0,
                                nullptr);

        return true;
    }

    VkDescriptorSetLayout VulkanPipeline::createDescriptorSetLayout(
        const std::vector<VkDescriptorSetLayoutBinding>& layoutBindings)
    {
        destroyDescriptorSetLayout();

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
        layoutInfo.pBindings = layoutBindings.data();

        if (vkCreateDescriptorSetLayout(m_logicalDevice, &layoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS)
        {
            LOGE("create descriptor set layout failed");
            return nullptr;
        }

        return m_descriptorSetLayout;
    }

    void VulkanPipeline::destroyDescriptorSetLayout()
    {
        if (m_descriptorSetLayout != nullptr)
        {
            vkDestroyDescriptorSetLayout(m_logicalDevice, m_descriptorSetLayout, nullptr);
            m_descriptorSetLayout = nullptr;
        }
    }

    VkPipeline VulkanPipeline::createGraphicsPipeline(const std::vector<uint8>& vertexShaderCode,
                                                      const std::vector<uint8>& fragmentShaderCode,
                                                      const std::vector<VkVertexInputBindingDescription>& bindings,
                                                      const std::vector<VkVertexInputAttributeDescription>& attributes,
                                                      VkPolygonMode polygonMode,
                                                      VkPrimitiveTopology topology)
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

        VkShaderModule vertShaderModule = createShaderModule(vertexShaderCode);
        VkShaderModule fragShaderModule = createShaderModule(fragmentShaderCode);

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
        inputAssembly.topology = topology;

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
        rasterizer.polygonMode = polygonMode;
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
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT
                                              | VK_COLOR_COMPONENT_A_BIT;
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

    VkShaderModule VulkanPipeline::createShaderModule(const std::vector<uint8>& shaderCode)
    {
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

    static VkFormat getVertexInputFormat(const spirv_cross::SPIRType& type, const tstring& name)
    {
        // TODO: find a better way to check the color input
        if (name == "inColor")
        {
            return VK_FORMAT_R8G8B8A8_UNORM;
        }
        else if (type.basetype == spirv_cross::SPIRType::BaseType::Float && type.width == 32)
        {
            if (type.vecsize == 1)
            {
                return VK_FORMAT_R32_SFLOAT;
            }
            else if (type.vecsize == 2)
            {
                return VK_FORMAT_R32G32_SFLOAT;
            }
            else if (type.vecsize == 3)
            {
                return VK_FORMAT_R32G32B32_SFLOAT;
            }
            else if (type.vecsize == 4)
            {
                return VK_FORMAT_R32G32B32A32_SFLOAT;
            }
        }

        return VK_FORMAT_UNDEFINED;
    }

    uint32 VulkanPipeline::generateVertexInputDesc(std::vector<VkVertexInputBindingDescription>& bindingDesc,
                                                   std::vector<VkVertexInputAttributeDescription>& attributeDesc,
                                                   const std::vector<uint8>& vertexShaderCode)
    {
        spirv_cross::CompilerGLSL glsl((uint32_t*)vertexShaderCode.data(), vertexShaderCode.size() / sizeof(uint32_t));
        auto resources = glsl.get_shader_resources();

        bindingDesc.resize(resources.stage_inputs.size());
        attributeDesc.resize(resources.stage_inputs.size());
        uint32 vertexInputMasks = 0;

        // vertex inputs
        for (int i = 0; i < resources.stage_inputs.size(); ++i)
        {
            const auto& input = resources.stage_inputs[i];

            // auto name = input.name;
            auto type = glsl.get_type(input.type_id);
            // auto baseType = glsl.get_type(input.base_type_id);

            auto location = glsl.get_decoration(input.id, spv::DecorationLocation);
            // auto binding = glsl.get_decoration(input.id, spv::DecorationBinding);

            attributeDesc[i].location = location;
            attributeDesc[i].binding = location;
            attributeDesc[i].format = getVertexInputFormat(type, input.name);
            attributeDesc[i].offset = 0;

            bindingDesc[i].binding = location;
            if (attributeDesc[i].format == VK_FORMAT_R8G8B8A8_UNORM)
            {
                bindingDesc[i].stride = 4;
            }
            else
            {
                bindingDesc[i].stride = (type.vecsize * type.width) >> 3;
            }
            bindingDesc[i].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            // TODO: validate location
            vertexInputMasks |= (1 << location);
        }

        return vertexInputMasks;
    }

    size_t VulkanPipeline::generateDescriptorSetLayout(std::vector<VkDescriptorSetLayoutBinding>& layoutBindings,
                                                       std::vector<VkWriteDescriptorSet>& writeSets,
                                                       const std::vector<uint8>& fragmentShaderCode)
    {
        layoutBindings.clear();
        writeSets.clear();

        spirv_cross::CompilerGLSL glsl((uint32_t*)fragmentShaderCode.data(), fragmentShaderCode.size() / sizeof(uint32_t));
        auto resources = glsl.get_shader_resources();

        // pfu
        {
            VkDescriptorSetLayoutBinding pfuLayoutBinding{};
            pfuLayoutBinding.binding = PerFrameUniformIndex;
            pfuLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            pfuLayoutBinding.descriptorCount = 1;
            pfuLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            layoutBindings.push_back(pfuLayoutBinding);

            VkWriteDescriptorSet pfuDescriptorWrites{};
            pfuDescriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            pfuDescriptorWrites.dstBinding = PerFrameUniformIndex;
            pfuDescriptorWrites.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            pfuDescriptorWrites.descriptorCount = 1;
            writeSets.push_back(pfuDescriptorWrites);
        }

        // pmu
        {
            VkDescriptorSetLayoutBinding pmuLayoutBinding{};
            pmuLayoutBinding.binding = PerMaterialUniformIndex;
            pmuLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            pmuLayoutBinding.descriptorCount = 1;
            pmuLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            layoutBindings.push_back(pmuLayoutBinding);

            VkWriteDescriptorSet pmuDescriptorWrites{};
            pmuDescriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            pmuDescriptorWrites.dstBinding = PerMaterialUniformIndex;
            pmuDescriptorWrites.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            pmuDescriptorWrites.descriptorCount = 1;
            writeSets.push_back(pmuDescriptorWrites);
        }

        // piu
        {
            VkDescriptorSetLayoutBinding piuLayoutBinding{};
            piuLayoutBinding.binding = PerInstanceUniformIndex;
            piuLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            piuLayoutBinding.descriptorCount = 1;
            piuLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            layoutBindings.push_back(piuLayoutBinding);

            VkWriteDescriptorSet piuDescriptorWrites{};
            piuDescriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            piuDescriptorWrites.dstBinding = PerInstanceUniformIndex;
            piuDescriptorWrites.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            piuDescriptorWrites.descriptorCount = 1;
            writeSets.push_back(piuDescriptorWrites);
        }

        // samples
        for (int i = 0; i < resources.sampled_images.size(); ++i)
        {
            const auto& sample = resources.sampled_images[i];

            // auto name = sample.name;
            auto type = glsl.get_type(sample.type_id);
            // auto baseType = glsl.get_type(sample.base_type_id);

            auto binding = glsl.get_decoration(sample.id, spv::DecorationBinding);

            VkDescriptorSetLayoutBinding samplerLayoutBinding{};
            samplerLayoutBinding.binding = binding;
            samplerLayoutBinding.descriptorCount = 1;
            samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            layoutBindings.push_back(samplerLayoutBinding);

            VkWriteDescriptorSet imageDescriptorWrites{};
            imageDescriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            imageDescriptorWrites.dstBinding = binding;
            imageDescriptorWrites.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            imageDescriptorWrites.descriptorCount = 1;
            writeSets.push_back(imageDescriptorWrites);
        }

        return resources.sampled_images.size();
    }
} // namespace blink
