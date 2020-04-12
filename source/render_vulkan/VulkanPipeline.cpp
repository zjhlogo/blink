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

NS_BEGIN

VulkanPipeline::VulkanPipeline()
{
}

VulkanPipeline::~VulkanPipeline()
{
}

bool VulkanPipeline::initialize(VulkanContext* context, VulkanLogicalDevice* logicalDevice, VulkanSwapchain* swapchain)
{
    m_context = context;
    m_logicalDevice = logicalDevice;
    m_swapchain = swapchain;

    auto format = m_swapchain->getImageFormat();
    const auto& extent = m_swapchain->getImageExtent();

    if (!createRenderPass(format, VulkanUtils::findDepthFormat(m_context->getPickedPhysicalDevice()))) return false;
    if (!createDescriptorSetLayout()) return false;
    if (!createGraphicsPipeline(extent.width, extent.height)) return false;

    return true;
}

void VulkanPipeline::terminate()
{
    destroyGraphicsPipeline();
    destroyDescriptorSetLayout();
    destroyRenderPass();
}

bool VulkanPipeline::createRenderPass(const vk::Format& colorAttachmentFormat, const vk::Format& depthAttachmentFormat)
{
    // color attachment
    vk::AttachmentDescription colorAttachment;
    colorAttachment.format = colorAttachmentFormat;
    colorAttachment.samples = vk::SampleCountFlagBits::e1;
    colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
    colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
    colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

    // depth attachment
    vk::AttachmentDescription depthAttachment;
    depthAttachment.format = depthAttachmentFormat;
    depthAttachment.samples = vk::SampleCountFlagBits::e1;
    depthAttachment.loadOp = vk::AttachmentLoadOp::eClear;
    depthAttachment.storeOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
    depthAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
    depthAttachment.initialLayout = vk::ImageLayout::eUndefined;
    depthAttachment.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

    // subpass
    vk::AttachmentReference colorAttachmentRef;
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

    vk::AttachmentReference depthAttachmentRef;
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

    vk::SubpassDescription subpass;
    subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    // create render pass
    std::array<vk::AttachmentDescription, 2> attacments = {colorAttachment, depthAttachment};
    vk::RenderPassCreateInfo renderPassInfo;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attacments.size());
    renderPassInfo.pAttachments = attacments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    vk::SubpassDependency dependency(VK_SUBPASS_EXTERNAL, 0);
    dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;

    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    const auto& logicalDevice = m_logicalDevice->getVkLogicalDevice();
    m_renderPass = logicalDevice.createRenderPass(renderPassInfo);

    return true;
}

void VulkanPipeline::destroyRenderPass()
{
    const auto& logicalDevice = m_logicalDevice->getVkLogicalDevice();
    logicalDevice.destroyRenderPass(m_renderPass);
}

bool VulkanPipeline::createDescriptorSetLayout()
{
    vk::DescriptorSetLayoutBinding uboLayoutBinding;
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eVertex;
    uboLayoutBinding.pImmutableSamplers = nullptr;

    vk::DescriptorSetLayoutBinding samplerLayoutBinding;
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = vk::DescriptorType::eCombinedImageSampler;
    samplerLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eFragment;
    samplerLayoutBinding.pImmutableSamplers = nullptr;

    std::array<vk::DescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};

    vk::DescriptorSetLayoutCreateInfo layoutInfo;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    const auto& logicalDevice = m_logicalDevice->getVkLogicalDevice();
    m_descriptorSetLayout = logicalDevice.createDescriptorSetLayout(layoutInfo);

    return true;
}

void VulkanPipeline::destroyDescriptorSetLayout()
{
    const auto& logicalDevice = m_logicalDevice->getVkLogicalDevice();
    logicalDevice.destroyDescriptorSetLayout(m_descriptorSetLayout);
}

bool VulkanPipeline::createGraphicsPipeline(uint32_t width, uint32_t height)
{
    const auto& logicalDevice = m_logicalDevice->getVkLogicalDevice();

    std::vector<uint8> vertShaderCode;
    File::readFileIntoBuffer(vertShaderCode, "resource/shaders/shader_base.vert.spv");

    std::vector<uint8> fragShaderCode;
    File::readFileIntoBuffer(fragShaderCode, "resource/shaders/shader_base.frag.spv");

    vk::ShaderModule vertShaderModule = createShaderModule(vertShaderCode);
    vk::ShaderModule fragShaderModule = createShaderModule(fragShaderCode);

    vk::PipelineShaderStageCreateInfo vertShaderStageInfo;
    vertShaderStageInfo.stage = vk::ShaderStageFlagBits::eVertex;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    vk::PipelineShaderStageCreateInfo fragShaderStageInfo;
    fragShaderStageInfo.stage = vk::ShaderStageFlagBits::eFragment;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    // shader state
    vk::PipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    // vertex input state
    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    // input assembly state
    vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
    inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;

    // viewport state
    vk::Viewport viewport(0.0f, 0.0f, width * 1.0f, height * 1.0f, 0.0f, 1.0f);
    vk::Rect2D sissor({0, 0}, {width, height});
    vk::PipelineViewportStateCreateInfo viewportState;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &sissor;

    // rasterizer state
    vk::PipelineRasterizationStateCreateInfo rasterizer;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = vk::PolygonMode::eFill;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = vk::CullModeFlagBits::eBack;
    rasterizer.frontFace = vk::FrontFace::eClockwise;
    rasterizer.depthBiasEnable = VK_FALSE;

    // multisampling state
    vk::PipelineMultisampleStateCreateInfo multisampling;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

    // depth and stencil state
    vk::PipelineDepthStencilStateCreateInfo depthStencil;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = vk::CompareOp::eLess;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;

    // color blending state
    vk::PipelineColorBlendAttachmentState colorBlendAttachment;
    colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB
                                          | vk::ColorComponentFlagBits::eA;
    colorBlendAttachment.blendEnable = VK_FALSE;

    vk::PipelineColorBlendStateCreateInfo colorBlending;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = vk::LogicOp::eCopy;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    // dynamic state
    //         VkDynamicState dynamicStates[] = {
    //             VK_DYNAMIC_STATE_VIEWPORT,
    //             VK_DYNAMIC_STATE_LINE_WIDTH
    //         };
    //
    //         VkPipelineDynamicStateCreateInfo dynamicState = {};
    //         dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    //         dynamicState.dynamicStateCount = 2;
    //         dynamicState.pDynamicStates = dynamicStates;

    // layout state
    vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &m_descriptorSetLayout;
    m_pipelineLayout = logicalDevice.createPipelineLayout(pipelineLayoutInfo);

    // create pipeline
    vk::GraphicsPipelineCreateInfo pipelineInfo;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    //        pipelineInfo.pDynamicState = nullptr;
    pipelineInfo.layout = m_pipelineLayout;
    pipelineInfo.renderPass = m_renderPass;
    pipelineInfo.subpass = 0;

    m_pipeline = logicalDevice.createGraphicsPipeline(vk::PipelineCache(), pipelineInfo);

    logicalDevice.destroyShaderModule(fragShaderModule);
    logicalDevice.destroyShaderModule(vertShaderModule);

    return true;
}

void VulkanPipeline::destroyGraphicsPipeline()
{
    const auto& logicalDevice = m_logicalDevice->getVkLogicalDevice();
    logicalDevice.destroyPipeline(m_pipeline);
    logicalDevice.destroyPipelineLayout(m_pipelineLayout);
}

vk::ShaderModule VulkanPipeline::createShaderModule(const std::vector<uint8>& shaderCode)
{
    vk::ShaderModuleCreateInfo createInfo;
    createInfo.codeSize = shaderCode.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

    const auto& logicalDevice = m_logicalDevice->getVkLogicalDevice();
    return logicalDevice.createShaderModule(createInfo);
}

NS_END
