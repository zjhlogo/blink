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
#include "VulkanLogicalDevice.h"
#include "VulkanRenderModule.h"
#include "VulkanRenderPass.h"
#include "utils/VulkanUtils.h"

#include <foundation/File.h>
#include <spirv-cross/spirv_glsl.hpp>

namespace blink
{
    VulkanPipeline::VulkanPipeline(VulkanLogicalDevice& logicalDevice, VulkanRenderPass& renderPass)
        : m_logicalDevice(logicalDevice)
        , m_renderPass(renderPass)
    {
        //
    }

    VulkanPipeline::~VulkanPipeline()
    {
        //
        destroy();
    }

    bool VulkanPipeline::create(const tstring& vertexShader, const tstring& fragmentShader, VkPolygonMode polygonMode, PrimitiveTopology topology)
    {
        if (m_pipeline != VK_NULL_HANDLE) return true;

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

        std::vector<uint8_t> vertexShaderCode;
        if (!File::readFileIntoBuffer(vertexShaderCode, m_vertexShader)) return false;

        std::vector<uint8_t> fragmentShaderCode;
        if (!File::readFileIntoBuffer(fragmentShaderCode, m_fragmentShader)) return false;

        // generate vertex input desc
        std::vector<VkVertexInputBindingDescription> vertexInputBindings;
        std::vector<VkVertexInputAttributeDescription> vertexInputAttributes;
        m_vertexAttrs = generateVertexInputDesc(vertexInputBindings, vertexInputAttributes, vertexShaderCode);

        // generate layout bindings
        std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
        generateDescriptorSetLayout(layoutBindings, vertexShaderCode, VK_SHADER_STAGE_VERTEX_BIT);
        generateDescriptorSetLayout(layoutBindings, fragmentShaderCode, VK_SHADER_STAGE_FRAGMENT_BIT);

        if (VK_NULL_HANDLE == createDescriptorSetLayout(layoutBindings))
        {
            return false;
        }

        auto renderModule = getRenderModule();
        const auto& surfaceSize = renderModule->getSurfaceSize();

        if (VK_NULL_HANDLE
            == createGraphicsPipeline(vertexShaderCode, fragmentShaderCode, vertexInputBindings, vertexInputAttributes, m_polygonMode, m_topology, surfaceSize))
        {
            return false;
        }

        return true;
    }

    void VulkanPipeline::destroy()
    {
        destroyGraphicsPipeline();
        destroyDescriptorSetLayout();

        m_writeSets.clear();
        m_uniformWriteSetIndexMap.clear();
        m_textureWriteSetIndexMap.clear();
        m_uniformBlocks.clear();
        m_vertexAttrs = VertexAttrs::None;
    }

    bool VulkanPipeline::bindDescriptorSets(const VulkanCommandBuffer& commandBuffer, const std::vector<VulkanPipeline::DescriptorInfo>& descriptorInfoList)
    {
        assert(m_writeSets.size() == descriptorInfoList.size());

        auto descriptorSet = m_logicalDevice.getDescriptorPool().allocateDescriptorSet(m_descriptorSetLayout);

        for (std::size_t i = 0; i < m_writeSets.size(); ++i)
        {
            auto& writeSet = m_writeSets[i];
            writeSet.dstSet = descriptorSet;

            if (writeSet.descriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
            {
                writeSet.pBufferInfo = &descriptorInfoList[i].bufferInfo;
            }
            else
            {
                writeSet.pBufferInfo = nullptr;
            }

            if (writeSet.descriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
            {
                writeSet.pImageInfo = &descriptorInfoList[i].imageInfo;
            }
            else
            {
                writeSet.pImageInfo = nullptr;
            }
        }

        vkUpdateDescriptorSets(m_logicalDevice, static_cast<uint32_t>(m_writeSets.size()), m_writeSets.data(), 0, nullptr);

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);

        return true;
    }

    VulkanUniformBlock* VulkanPipeline::getUniformBlock(UniformBinding binding)
    {
        auto it = m_uniformBlocks.find(static_cast<uint32_t>(binding));
        if (it == m_uniformBlocks.end()) return nullptr;

        return &it->second;
    }

    int VulkanPipeline::getUniformWriteSetIndexFromBinding(UniformBinding binding)
    {
        auto it = m_uniformWriteSetIndexMap.find(static_cast<uint32_t>(binding));
        if (it == m_uniformWriteSetIndexMap.end()) return -1;

        return it->second;
    }

    VkDescriptorSetLayout VulkanPipeline::createDescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding>& layoutBindings)
    {
        destroyDescriptorSetLayout();

        if (layoutBindings.empty()) return VK_NULL_HANDLE;

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
        layoutInfo.pBindings = layoutBindings.data();

        VK_CHECK_RESULT(vkCreateDescriptorSetLayout(m_logicalDevice, &layoutInfo, nullptr, &m_descriptorSetLayout))
        return m_descriptorSetLayout;
    }

    void VulkanPipeline::destroyDescriptorSetLayout()
    {
        if (m_descriptorSetLayout != VK_NULL_HANDLE)
        {
            vkDestroyDescriptorSetLayout(m_logicalDevice, m_descriptorSetLayout, nullptr);
            m_descriptorSetLayout = VK_NULL_HANDLE;
        }
    }

    VkPipeline VulkanPipeline::createGraphicsPipeline(const std::vector<uint8_t>& vertexShaderCode,
                                                      const std::vector<uint8_t>& fragmentShaderCode,
                                                      const std::vector<VkVertexInputBindingDescription>& bindings,
                                                      const std::vector<VkVertexInputAttributeDescription>& attributes,
                                                      VkPolygonMode polygonMode,
                                                      VkPrimitiveTopology topology,
                                                      const glm::ivec2& surfaceSize)
    {
        destroyGraphicsPipeline();

        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(PushConstantData);

        // layout state
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;
        pipelineLayoutInfo.pSetLayouts = &m_descriptorSetLayout;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        VK_CHECK_RESULT(vkCreatePipelineLayout(m_logicalDevice, &pipelineLayoutInfo, nullptr, &m_pipelineLayout))

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
        VkViewport viewport{0.0f, 0.0f, static_cast<float>(surfaceSize.x), static_cast<float>(surfaceSize.y), 0.0f, 1.0f};
        VkRect2D scissor{{0, 0}, {static_cast<uint32_t>(surfaceSize.x), static_cast<uint32_t>(surfaceSize.y)}};
        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        // rasterizer state
        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = polygonMode;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_NONE;
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
        pipelineInfo.renderPass = m_renderPass;
        pipelineInfo.subpass = 0;

        VK_CHECK_RESULT(vkCreateGraphicsPipelines(m_logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline))
        vkDestroyShaderModule(m_logicalDevice, fragShaderModule, nullptr);
        vkDestroyShaderModule(m_logicalDevice, vertShaderModule, nullptr);

        return m_pipeline;
    }

    void VulkanPipeline::destroyGraphicsPipeline()
    {
        if (m_pipeline != VK_NULL_HANDLE)
        {
            vkDestroyPipeline(m_logicalDevice, m_pipeline, nullptr);
            m_pipeline = VK_NULL_HANDLE;
        }

        if (m_pipelineLayout != VK_NULL_HANDLE)
        {
            vkDestroyPipelineLayout(m_logicalDevice, m_pipelineLayout, nullptr);
            m_pipelineLayout = VK_NULL_HANDLE;
        }
    }

    VkShaderModule VulkanPipeline::createShaderModule(const std::vector<uint8_t>& shaderCode)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = shaderCode.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

        VkShaderModule shaderModule{};
        VK_CHECK_RESULT(vkCreateShaderModule(m_logicalDevice, &createInfo, nullptr, &shaderModule))
        return shaderModule;
    }

    static VkFormat getVertexInputFormat(const spirv_cross::SPIRType& type, const tstring& name)
    {
        // TODO: find a better way to check the color input
        if (name == "inColor")
        {
            return VK_FORMAT_R8G8B8A8_UNORM;
        }
        if (type.basetype == spirv_cross::SPIRType::BaseType::Float && type.width == 32)
        {
            if (type.vecsize == 1)
            {
                return VK_FORMAT_R32_SFLOAT;
            }
            if (type.vecsize == 2)
            {
                return VK_FORMAT_R32G32_SFLOAT;
            }
            if (type.vecsize == 3)
            {
                return VK_FORMAT_R32G32B32_SFLOAT;
            }
            if (type.vecsize == 4)
            {
                return VK_FORMAT_R32G32B32A32_SFLOAT;
            }
        }

        return VK_FORMAT_UNDEFINED;
    }

    VertexAttrs VulkanPipeline::generateVertexInputDesc(std::vector<VkVertexInputBindingDescription>& bindingDesc,
                                                        std::vector<VkVertexInputAttributeDescription>& attributeDesc,
                                                        const std::vector<uint8_t>& vertexShaderCode)
    {
        spirv_cross::CompilerGLSL glsl(reinterpret_cast<const uint32_t*>(vertexShaderCode.data()), vertexShaderCode.size() / sizeof(uint32_t));
        auto resources = glsl.get_shader_resources();

        bindingDesc.resize(resources.stage_inputs.size());
        attributeDesc.resize(resources.stage_inputs.size());
        VertexAttrs vertexAttrs{};

        // vertex inputs
        for (int i = 0; i < static_cast<int>(resources.stage_inputs.size()); ++i)
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

            vertexAttrs.set(1 << location);
        }

        return vertexAttrs;
    }

    void VulkanPipeline::generateDescriptorSetLayout(std::vector<VkDescriptorSetLayoutBinding>& layoutBindings,
                                                     const std::vector<uint8_t>& shaderCode,
                                                     uint32_t shaderStageBits)
    {
        spirv_cross::CompilerGLSL glsl(reinterpret_cast<const uint32_t*>(shaderCode.data()), shaderCode.size() / sizeof(uint32_t));
        auto resources = glsl.get_shader_resources();

        // uniform buffers
        for (int i = 0; i < static_cast<int>(resources.uniform_buffers.size()); ++i)
        {
            const auto& uniform = resources.uniform_buffers[i];

            // auto type = glsl.get_type(uniform.type_id);
            auto baseType = glsl.get_type(uniform.base_type_id);

            // auto set = glsl.get_decoration(uniform.id, spv::DecorationDescriptorSet);
            auto binding = glsl.get_decoration(uniform.id, spv::DecorationBinding);

            auto indexIt = m_uniformWriteSetIndexMap.find(binding);
            if (indexIt != m_uniformWriteSetIndexMap.end())
            {
                layoutBindings[indexIt->second].stageFlags |= shaderStageBits;
                continue;
            }

            VkDescriptorSetLayoutBinding layoutBinding{};
            layoutBinding.binding = binding;
            layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            layoutBinding.descriptorCount = 1;
            layoutBinding.stageFlags = shaderStageBits;
            layoutBindings.push_back(layoutBinding);

            VkWriteDescriptorSet descriptorWrites{};
            descriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites.dstBinding = binding;
            descriptorWrites.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites.descriptorCount = 1;

            m_uniformWriteSetIndexMap.insert({binding, static_cast<int>(m_writeSets.size())});
            m_writeSets.push_back(descriptorWrites);

            auto pair = m_uniformBlocks.insert({binding, {}});
            auto& uniformBlock = pair.first->second;
            // auto structSize = glsl.get_declared_struct_size(baseType);
            for (int j = 0; j < static_cast<int>(baseType.member_types.size()); ++j)
            {
                auto memberTypeId = baseType.member_types[j];
                auto memberType = glsl.get_type(memberTypeId);
                const auto& memberName = glsl.get_member_name(uniform.base_type_id, j);
                auto memberOffset = glsl.get_member_decoration(uniform.base_type_id, j, spv::DecorationOffset);
                auto memberBaseType = memberType.basetype; // float, int ...
                auto rowSize = memberType.vecsize;         // row
                auto colSize = memberType.columns;         // col

                if (memberBaseType == spirv_cross::SPIRType::BaseType::Int && rowSize == 1 && colSize == 1)
                {
                    uniformBlock.addUniformMember(memberName, UniformType::Int, memberOffset);
                }
                else if (memberBaseType == spirv_cross::SPIRType::BaseType::Float && rowSize == 1 && colSize == 1)
                {
                    uniformBlock.addUniformMember(memberName, UniformType::Float, memberOffset);
                }
                else if (memberBaseType == spirv_cross::SPIRType::BaseType::Float && rowSize == 2 && colSize == 1)
                {
                    uniformBlock.addUniformMember(memberName, UniformType::Vec2, memberOffset);
                }
                else if (memberBaseType == spirv_cross::SPIRType::BaseType::Float && rowSize == 3 && colSize == 1)
                {
                    uniformBlock.addUniformMember(memberName, UniformType::Vec3, memberOffset);
                }
                else if (memberBaseType == spirv_cross::SPIRType::BaseType::Float && rowSize == 4 && colSize == 1)
                {
                    uniformBlock.addUniformMember(memberName, UniformType::Vec4, memberOffset);
                }
                else if (memberBaseType == spirv_cross::SPIRType::BaseType::Float && rowSize == 3 && colSize == 3)
                {
                    uniformBlock.addUniformMember(memberName, UniformType::Mat3, memberOffset);
                }
                else if (memberBaseType == spirv_cross::SPIRType::BaseType::Float && rowSize == 4 && colSize == 4)
                {
                    uniformBlock.addUniformMember(memberName, UniformType::Mat4, memberOffset);
                }
            }
        }

        // uniform samples
        for (int i = 0; i < static_cast<int>(resources.sampled_images.size()); ++i)
        {
            const auto& sample = resources.sampled_images[i];

            // auto type = glsl.get_type(sample.type_id);
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

            m_textureWriteSetIndexMap.insert({sample.name, static_cast<int>(m_writeSets.size())});
            m_writeSets.push_back(imageDescriptorWrites);
        }
    }
} // namespace blink
