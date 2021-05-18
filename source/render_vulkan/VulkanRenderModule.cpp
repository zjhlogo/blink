/*!
 * \file VulkanRenderModule.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 *
 */
#include "VulkanRenderModule.h"
#include "Types.h"
#include "VulkanContext.h"
#include "VulkanLogicalDevice.h"
#include "VulkanPipeline.h"
#include "VulkanSwapchain.h"
#include "VulkanTexture.h"
#include "VulkanWindow.h"
#include "utils/VulkanUtils.h"

#include <foundation/File.h>
#include <render_base/util/stb_image.h>

#include <chrono>
#include <set>

NS_BEGIN

const std::vector<Vertex> g_vertices = {{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                                        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
                                        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
                                        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

                                        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                                        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
                                        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
                                        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}};

const std::vector<uint16_t> g_indices = {0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4};

struct UniformBufferObject
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

VulkanRenderModule::VulkanRenderModule()
    : RenderModule("Vulkan")
{
}

VulkanRenderModule::~VulkanRenderModule()
{
}

bool VulkanRenderModule::createDevice(const glm::ivec2& deviceSize)
{
    /* Initialize the library */
    if (!glfwInit()) return false;

    m_window = new VulkanWindow();
    if (!m_window->initialize(deviceSize)) return false;

    m_context = new VulkanContext();
    if (!m_context->initialize(m_window)) return false;

    m_logicalDevice = new VulkanLogicalDevice();
    if (!m_logicalDevice->initialize(m_context)) return false;

    m_swapchain = new VulkanSwapchain();
    if (!m_swapchain->initialize(m_window, m_context, m_logicalDevice->getVkLogicalDevice())) return false;

    m_pipeline = new VulkanPipeline();
    if (!m_pipeline->initialize(m_context, m_logicalDevice, m_swapchain)) return false;

    if (!createCommandPool()) return false;

    const auto& extent = m_swapchain->getImageExtent();
    m_depthTexture = createDepthTexture(extent.width, extent.height);

    if (!m_swapchain->createFramebuffers((VulkanTexture*)m_depthTexture, m_pipeline->getRenderPass())) return false;

    m_texture = createTexture2D("resource/texture.jpg");

    if (!m_logicalDevice->createVertexBuffer(m_vertexBuffer, m_vertexBufferMemory, m_commandPool, g_vertices.data(), g_vertices.size())) return false;
    if (!m_logicalDevice->createVertexBuffer(m_indexBuffer, m_indexBufferMemory, m_commandPool, g_indices.data(), g_indices.size())) return false;
    if (!createUniformBuffers()) return false;
    if (!createDescriptorPool()) return false;
    if (!createDescriptorSets()) return false;
    if (!createCommandBuffers()) return false;
    if (!createSyncObjects()) return false;

    return true;
}

void VulkanRenderModule::destroyDevice()
{
    m_swapchain->destroyFramebuffers();
    destroyTexture(m_depthTexture);
    destroyCommandBuffers();

    destroyTexture(m_texture);

    SAFE_DELETE_AND_TERMINATE(m_pipeline);

    destroyUniformBuffers();
    destroyDescriptorSets();
    destroyDescriptorPool();
    m_logicalDevice->destroyBuffer(m_indexBuffer, m_indexBufferMemory);
    m_logicalDevice->destroyBuffer(m_vertexBuffer, m_vertexBufferMemory);
    destroySyncObjects();
    destroyCommandPool();
    SAFE_DELETE_AND_TERMINATE(m_logicalDevice);
    SAFE_DELETE_AND_TERMINATE(m_context);
    SAFE_DELETE_AND_TERMINATE(m_window);

    glfwTerminate();
}

Texture* VulkanRenderModule::createTexture2D(const tstring& texFile)
{
    int texWidth = 0;
    int texHeight = 0;
    int texChannels = 0;

    stbi_uc* pixels = stbi_load(texFile.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    if (!pixels) return nullptr;

    VulkanTexture* vulkanTexture = new VulkanTexture(this);
    bool success = vulkanTexture->createTexture2D(pixels, texWidth, texHeight, texChannels);
    stbi_image_free(pixels);

    if (!success)
    {
        delete vulkanTexture;
        vulkanTexture = nullptr;
    }

    return vulkanTexture;
}

Texture* VulkanRenderModule::createDepthTexture(int width, int height)
{
    VulkanTexture* vulkanTexture = new VulkanTexture(this);
    if (!vulkanTexture->createDepthTexture(width, height))
    {
        delete vulkanTexture;
        vulkanTexture = nullptr;
    }

    return vulkanTexture;
}

bool VulkanRenderModule::destroyTexture(Texture*& texture)
{
    if (texture)
    {
        VulkanTexture* vulkanTexture = (VulkanTexture*)texture;
        vulkanTexture->destroy();
        delete vulkanTexture;
        texture = nullptr;
    }

    return true;
}

bool VulkanRenderModule::gameLoop()
{
    double begin = glfwGetTime();

    /* Loop until the user closes the window */
    if (glfwWindowShouldClose(m_window->getWindow())) return false;

    /* Poll for and process events */
    glfwPollEvents();

    double end = glfwGetTime();
    double duration = end - begin;
    begin = end;

    // app->step(static_cast<float>(duration));

    drawFrame();

    m_logicalDevice.waitIdle();

    return true;
}

void VulkanRenderModule::drawFrame()
{
    m_logicalDevice.waitForFences(m_inFlightFences[m_currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

    uint32_t imageIndex{};
    vk::Result result = m_logicalDevice.acquireNextImageKHR(m_swapChain,
                                                            std::numeric_limits<uint64_t>::max(),
                                                            m_imageAvailableSemaphores[m_currentFrame],
                                                            vk::Fence(),
                                                            &imageIndex);

    if (result == vk::Result::eErrorOutOfDateKHR)
    {
        assert(0 && "not support yet");
        //         m_swapchain->recreateSwapChain();
        //         m_pipeline->createRenderPass(m_swapChainImageFormat, VulkanUtils::findDepthFormat(m_context->getPickedPhysicalDevice()));
        //         m_pipeline->createGraphicsPipeline(m_swapChainExtent.width, m_swapChainExtent.height);
        //
        //         m_depthTexture = createDepthTexture(m_swapChainExtent.width, m_swapChainExtent.height);
        //
        //         createFramebuffers();
        //         createUniformBuffers();
        //         createDescriptorPool();
        //         createDescriptorSets();
        //         createCommandBuffers();

        return;
    }
    else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
    {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    updateUniformBuffer(imageIndex);

    if (m_imagesInFlight[imageIndex])
    {
        m_logicalDevice.waitForFences(m_imagesInFlight[imageIndex], VK_TRUE, std::numeric_limits<uint64_t>::max());
    }
    m_imagesInFlight[imageIndex] = m_inFlightFences[m_currentFrame];

    vk::SubmitInfo submitInfo;
    vk::Semaphore waitSemaphores[] = {m_imageAvailableSemaphores[m_currentFrame]};
    vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_commandBuffers[imageIndex];

    vk::Semaphore signalSemaphores[] = {m_renderFinishedSemaphores[m_currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    m_logicalDevice.resetFences(m_inFlightFences[m_currentFrame]);
    m_graphicsQueue.submit(submitInfo, m_inFlightFences[m_currentFrame]);

    vk::PresentInfoKHR presentInfo;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    vk::SwapchainKHR swapChains[] = {m_swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    result = m_presentQueue.presentKHR(&presentInfo);
    if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || m_frameBufferResized)
    {
        assert(0 && "not support yet");

        //         m_frameBufferResized = false;
        //         m_swapchain->recreateSwapChain();
        //
        //         createImageViews();
        //         m_pipeline->createRenderPass(m_swapChainImageFormat, VulkanUtils::findDepthFormat(m_context->getPickedPhysicalDevice()));
        //         m_pipeline->createGraphicsPipeline(m_swapChainExtent.width, m_swapChainExtent.height);
        //
        //         m_depthTexture = createDepthTexture(m_swapChainExtent.width, m_swapChainExtent.height);
        //
        //         createFramebuffers();
        //         createUniformBuffers();
        //         createDescriptorPool();
        //         createDescriptorSets();
        //         createCommandBuffers();
    }
    else if (result != vk::Result::eSuccess)
    {
        throw std::runtime_error("failed to present swap chain image!");
    }

    m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

Shader* VulkanRenderModule::createShaderFromBuffer(const char* vsBuffer, const char* gsBuffer, const char* fsBuffer)
{
    // TODO:
    return nullptr;
}

bool VulkanRenderModule::createUniformBuffers()
{
    auto imageCount = m_swapchain->getImageCount();

    vk::DeviceSize bufferSize = sizeof(UniformBufferObject);
    m_uniformBuffers.resize(imageCount);
    m_uniformBuffersMemory.resize(imageCount);

    for (int i = 0; i < imageCount; ++i)
    {
        m_logicalDevice->createBuffer(m_uniformBuffers[i],
                                      m_uniformBuffersMemory[i],
                                      bufferSize,
                                      vk::BufferUsageFlagBits::eUniformBuffer,
                                      vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
    }

    return true;
}

void VulkanRenderModule::destroyUniformBuffers()
{
    auto imageCount = m_swapchain->getImageCount();

    for (int i = 0; i < imageCount; ++i)
    {
        m_logicalDevice->destroyBuffer(m_uniformBuffers[i], m_uniformBuffersMemory[i]);
    }
}

bool VulkanRenderModule::createDescriptorPool()
{
    std::array<vk::DescriptorPoolSize, 2> poolSizes;
    poolSizes[0].type = vk::DescriptorType::eUniformBuffer;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(m_swapChainImages.size());
    poolSizes[1].type = vk::DescriptorType::eCombinedImageSampler;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(m_swapChainImages.size());

    vk::DescriptorPoolCreateInfo poolInfo;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(m_swapChainImages.size());
    m_descriptorPool = m_logicalDevice.createDescriptorPool(poolInfo);

    return true;
}

void VulkanRenderModule::destroyDescriptorPool()
{
    m_logicalDevice.destroyDescriptorPool(m_descriptorPool);
}

bool VulkanRenderModule::createDescriptorSets()
{
    std::vector<vk::DescriptorSetLayout> layouts(m_swapChainImages.size(), m_pipeline->getDestriptorSetLayout());

    vk::DescriptorSetAllocateInfo allocInfo;
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(m_swapChainImages.size());
    allocInfo.pSetLayouts = layouts.data();
    m_descriptorSets.resize(m_swapChainImages.size());
    m_descriptorSets = m_logicalDevice.allocateDescriptorSets(allocInfo);

    VulkanTexture* vulkanTexture = (VulkanTexture*)m_texture;

    for (size_t i = 0; i < m_swapChainImages.size(); ++i)
    {
        vk::DescriptorBufferInfo bufferInfo;
        bufferInfo.buffer = m_uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        vk::DescriptorImageInfo imageInfo;
        imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
        imageInfo.imageView = vulkanTexture->getTextureImageView();
        imageInfo.sampler = vulkanTexture->getTextureSampler();

        std::array<vk::WriteDescriptorSet, 2> descriptorWrites;
        descriptorWrites[0].dstSet = m_descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = vk::DescriptorType::eUniformBuffer;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        descriptorWrites[1].dstSet = m_descriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = vk::DescriptorType::eCombinedImageSampler;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;

        m_logicalDevice.updateDescriptorSets(static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }

    return true;
}

void VulkanRenderModule::destroyDescriptorSets()
{
}

bool VulkanRenderModule::createCommandBuffers()
{
    vk::CommandBufferAllocateInfo allocInfo;
    allocInfo.commandPool = m_commandPool;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandBufferCount = static_cast<uint32_t>(m_swapChainFramebuffers.size());
    m_commandBuffers = m_logicalDevice.allocateCommandBuffers(allocInfo);

    for (size_t i = 0; i < m_commandBuffers.size(); ++i)
    {
        vk::CommandBufferBeginInfo beginInfo;
        beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;

        m_commandBuffers[i].begin(beginInfo);

        {
            vk::RenderPassBeginInfo renderPassInfo;
            renderPassInfo.renderPass = m_pipeline->getRenderPass();
            renderPassInfo.framebuffer = m_swapChainFramebuffers[i];
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = m_swapChainExtent;

            std::array<vk::ClearValue, 2> clearValues;
            clearValues[0].color.setFloat32({0.0f, 0.0f, 0.0f, 1.0f});
            clearValues[1].depthStencil = {1.0f, 0};
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            m_commandBuffers[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

            {
                m_commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, m_pipeline->getPipeline());
                vk::DeviceSize offset = 0;
                m_commandBuffers[i].bindVertexBuffers(0, m_vertexBuffer, offset);
                m_commandBuffers[i].bindIndexBuffer(m_indexBuffer, 0, vk::IndexType::eUint16);
                m_commandBuffers[i]
                    .bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_pipeline->getPipelineLayout(), 0, 1, &m_descriptorSets[i], 0, nullptr);
                m_commandBuffers[i].drawIndexed(static_cast<uint32_t>(g_indices.size()), 1, 0, 0, 0);
            }

            m_commandBuffers[i].endRenderPass();
        }

        m_commandBuffers[i].end();
    }

    return true;
}

void VulkanRenderModule::destroyCommandBuffers()
{
    m_logicalDevice.freeCommandBuffers(m_commandPool, m_commandBuffers);
}

bool VulkanRenderModule::createSyncObjects()
{
    m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    m_imagesInFlight.resize(m_swapChainImages.size());

    vk::SemaphoreCreateInfo semaphoreInfo;
    vk::FenceCreateInfo fenceInfo;
    fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;

    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    {
        m_imageAvailableSemaphores[i] = m_logicalDevice.createSemaphore(semaphoreInfo);
        m_renderFinishedSemaphores[i] = m_logicalDevice.createSemaphore(semaphoreInfo);
        m_inFlightFences[i] = m_logicalDevice.createFence(fenceInfo);
    }

    return true;
}

void VulkanRenderModule::destroySyncObjects()
{
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    {
        m_logicalDevice.destroySemaphore(m_renderFinishedSemaphores[i]);
        m_logicalDevice.destroySemaphore(m_imageAvailableSemaphores[i]);
        m_logicalDevice.destroyFence(m_inFlightFences[i]);
    }
}

void VulkanRenderModule::updateUniformBuffer(uint32_t currentImage)
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo;
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), m_swapChainExtent.width / (float)m_swapChainExtent.height, 0.1f, 10.0f);

    void* data = m_logicalDevice.mapMemory(m_uniformBuffersMemory[currentImage], 0, sizeof(ubo));
    memcpy(data, &ubo, sizeof(ubo));
    m_logicalDevice.unmapMemory(m_uniformBuffersMemory[currentImage]);
}

NS_END
