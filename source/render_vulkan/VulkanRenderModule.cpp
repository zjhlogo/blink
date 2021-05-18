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
#include "VulkanBuffer.h"
#include "VulkanCommandPool.h"
#include "VulkanContext.h"
#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSets.h"
#include "VulkanLogicalDevice.h"
#include "VulkanMemory.h"
#include "VulkanPipeline.h"
#include "VulkanSwapchain.h"
#include "VulkanTexture.h"
#include "VulkanWindow.h"
#include "utils/VulkanUtils.h"

#include <foundation/File.h>
#include <glm/ext/matrix_clip_space.hpp>
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
    if (!m_window->create(deviceSize)) return false;

    m_context = new VulkanContext(m_window);
    if (!m_context->create()) return false;

    m_logicalDevice = new VulkanLogicalDevice(m_context);
    if (!m_logicalDevice->create()) return false;

    m_swapchain = new VulkanSwapchain(*m_logicalDevice);
    if (!m_swapchain->create()) return false;

    m_pipeline = new VulkanPipeline(*m_logicalDevice, *m_swapchain);
    if (!m_pipeline->create()) return false;

    m_commandPool = new VulkanCommandPool(*m_logicalDevice);
    if (!m_commandPool->create()) return false;

    const auto& extent = m_swapchain->getImageExtent();
    m_depthTexture = createDepthTexture(extent.width, extent.height);

    if (!m_swapchain->createFramebuffers((VulkanTexture*)m_depthTexture, m_pipeline->getRenderPass())) return false;

    m_texture = createTexture2D("resource/texture.jpg");

    m_vertexBuffer = new VulkanBuffer(*m_logicalDevice);
    if (!m_vertexBuffer->createBufferAndUpload((void*)g_vertices.data(),
                                               g_vertices.size(),
                                               VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                               VK_SHARING_MODE_EXCLUSIVE,
                                               *m_commandPool))
    {
        return false;
    }

    m_indexBuffer = new VulkanBuffer(*m_logicalDevice);
    if (!m_indexBuffer->createBufferAndUpload((void*)g_indices.data(),
                                              g_indices.size(),
                                              VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                                              VK_SHARING_MODE_EXCLUSIVE,
                                              *m_commandPool))
    {
        return false;
    }

    m_uniformBuffers.resize(m_swapchain->getImageCount());
    for (int i = 0; i < m_uniformBuffers.size(); ++i)
    {
        m_uniformBuffers[i] = new VulkanBuffer(*m_logicalDevice);
        if (!m_uniformBuffers[i]->createBuffer(sizeof(UniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE))
        {
            return false;
        }
        m_uniformBuffers[i]->allocateBufferMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    }

    m_descriptorPool = new VulkanDescriptorPool(*m_logicalDevice);
    if (!m_descriptorPool->create(m_swapchain->getImageCount())) return false;

    m_descriptorSets = new VulkanDescriptorSets(*m_logicalDevice, *m_descriptorPool, *m_pipeline);
    if (!m_descriptorSets->create(m_swapchain->getImageCount(), m_uniformBuffers, (VulkanTexture*)m_texture)) return false;

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

    SAFE_DELETE(m_pipeline);

    for (auto uniformBuffer : m_uniformBuffers)
    {
        SAFE_DELETE(uniformBuffer);
    }
    m_uniformBuffers.clear();

    SAFE_DELETE(m_descriptorSets);
    SAFE_DELETE(m_descriptorPool);
    SAFE_DELETE(m_indexBuffer);
    SAFE_DELETE(m_vertexBuffer);
    destroySyncObjects();
    SAFE_DELETE(m_commandPool);
    SAFE_DELETE(m_logicalDevice);
    SAFE_DELETE(m_context);
    SAFE_DELETE(m_window);

    glfwTerminate();
}

Texture* VulkanRenderModule::createTexture2D(const tstring& texFile)
{
    int texWidth = 0;
    int texHeight = 0;
    int texChannels = 0;

    stbi_uc* pixels = stbi_load(texFile.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    if (!pixels) return nullptr;

    VulkanTexture* vulkanTexture = new VulkanTexture(*m_logicalDevice, *m_commandPool);
    bool success = vulkanTexture->createTexture2D(pixels, texWidth, texHeight, texChannels);
    stbi_image_free(pixels);

    if (!success)
    {
        SAFE_DELETE(vulkanTexture);
    }

    return vulkanTexture;
}

Texture* VulkanRenderModule::createDepthTexture(int width, int height)
{
    VulkanTexture* vulkanTexture = new VulkanTexture(*m_logicalDevice, *m_commandPool);
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
    if (glfwWindowShouldClose(*m_window)) return false;

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

    const auto& extent = m_swapchain->getImageExtent();
    ubo.proj = glm::perspective(glm::radians(45.0f), extent.width / (float)extent.height, 0.1f, 10.0f);

    m_uniformBuffers[currentImage]->getBufferMemory()->uploadData(&ubo, sizeof(ubo));
}

NS_END
