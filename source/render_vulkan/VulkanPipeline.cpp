/*!
 * \file VulkanPipeline.cpp
 *
 * \author zjhlogo
 * \date 2020/02/26
 *
 *
 */
#include "VulkanPipeline.h"
#include "Types.h"
#include "VulkanContext.h"
#include "VulkanLogicalDevice.h"
#include "VulkanSwapchain.h"
#include "utils/VulkanUtils.h"

#include <foundation/File.h>
#include <foundation/Log.h>

namespace blink
{
    VulkanPipeline::VulkanPipeline(VulkanLogicalDevice& logicalDevice, VulkanSwapchain& swapchain)
        : m_logicalDevice(logicalDevice)
        , m_swapchain(swapchain)
    {
    }

    VulkanPipeline::~VulkanPipeline() { destroy(); }

    bool VulkanPipeline::create()
    {
        const auto& extent = m_swapchain.getImageExtent();

        if (!createDescriptorSetLayout()) return false;
        if (!createGraphicsPipeline(extent.width, extent.height)) return false;

        return true;
    }

    void VulkanPipeline::destroy()
    {
        destroyGraphicsPipeline();
        destroyDescriptorSetLayout();
    }

    bool VulkanPipeline::recreatePipeline()
    {
        const auto& extent = m_swapchain.getImageExtent();
        if (!createGraphicsPipeline(extent.width, extent.height)) return false;

        return true;
    }

    VkDescriptorSetLayout VulkanPipeline::createDescriptorSetLayout()
    {
        destroyDescriptorSetLayout();

        VkDescriptorSetLayoutBinding pfoLayoutBinding{};
        pfoLayoutBinding.binding = 0;
        pfoLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        pfoLayoutBinding.descriptorCount = 1;
        pfoLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutBinding pioLayoutBinding{};
        pioLayoutBinding.binding = 1;
        pioLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        pioLayoutBinding.descriptorCount = 1;
        pioLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 2;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        VkDescriptorSetLayoutBinding bindings[3] = {pfoLayoutBinding, pioLayoutBinding, samplerLayoutBinding};

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 3;
        layoutInfo.pBindings = bindings;

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

    VkPipeline VulkanPipeline::createGraphicsPipeline(uint32_t width, uint32_t height)
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

        VkShaderModule vertShaderModule = createShaderModule("resource/shaders/shader_base.vert.spv");
        VkShaderModule fragShaderModule = createShaderModule("resource/shaders/shader_base.frag.spv");

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

        const auto& bindingDesc = VertexPosColorUv1::getBindingDescription();
        vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDesc.size());
        vertexInputInfo.pVertexBindingDescriptions = bindingDesc.data();

        const auto& attributeDesc = VertexPosColorUv1::getAttributeDescriptions();
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDesc.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDesc.data();

        // input assembly state
        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        // viewport state
        VkViewport viewport{0.0f, 0.0f, width * 1.0f, height * 1.0f, 0.0f, 1.0f};
        VkRect2D sissor{{0, 0}, {width, height}};
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
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_FRONT_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
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

        //// dynamic state
        // VkDynamicState dynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH};
        // VkPipelineDynamicStateCreateInfo dynamicState = {};
        // dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        // dynamicState.dynamicStateCount = 2;
        // dynamicState.pDynamicStates = dynamicStates;

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
        // pipelineInfo.pDynamicState = &dynamicState;
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
