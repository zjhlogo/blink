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
#include "VulkanCommandBuffer.h"
#include "VulkanCommandPool.h"
#include "VulkanContext.h"
#include "VulkanDescriptorPool.h"
#include "VulkanFence.h"
#include "VulkanLogicalDevice.h"
#include "VulkanPipeline.h"
#include "VulkanSemaphore.h"
#include "VulkanSwapchain.h"
#include "VulkanWindow.h"
#include "utils/VulkanUtils.h"

#include <foundation/Log.h>

NS_BEGIN

VulkanRenderModule::VulkanRenderModule()
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

    m_commandPool = new VulkanCommandPool(*m_logicalDevice);
    if (!m_commandPool->create()) return false;

    m_swapchain = new VulkanSwapchain(*m_window, *m_logicalDevice, *m_commandPool);
    if (!m_swapchain->create()) return false;

    m_descriptorPool = new VulkanDescriptorPool(*m_logicalDevice);
    if (!m_descriptorPool->create((uint32_t)m_swapchain->getImageCount())) return false;

    if (!createCommandBuffers()) return false;
    if (!createSyncObjects()) return false;

    return true;
}

void VulkanRenderModule::destroyDevice()
{
    destroySyncObjects();
    destroyCommandBuffers();

    SAFE_DELETE(m_descriptorPool);
    SAFE_DELETE(m_swapchain);
    SAFE_DELETE(m_commandPool);
    SAFE_DELETE(m_logicalDevice);
    SAFE_DELETE(m_context);
    SAFE_DELETE(m_window);

    glfwTerminate();
}

bool VulkanRenderModule::gameLoop()
{
    double begin = glfwGetTime();

    /* Loop until the user closes the window */
    if (glfwWindowShouldClose(*m_window))
    {
        m_logicalDevice->waitIdle();
        return false;
    }

    /* Poll for and process events */
    glfwPollEvents();

    double end = glfwGetTime();
    double duration = end - begin;
    begin = end;

    // app->step(static_cast<float>(duration));

    drawFrame();

    return true;
}

void VulkanRenderModule::drawFrame()
{
    m_inFlightFences[m_currentFrame]->wait();
    m_inFlightFences[m_currentFrame]->reset();

    uint32_t imageIndex{};
    auto result = vkAcquireNextImageKHR(*m_logicalDevice, *m_swapchain, UINT64_MAX, *m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        // m_swapchain->recreateSwapChain();
        // m_pipeline->recreatePipeline();

        // const auto& extent = m_swapchain->getImageExtent();
        // m_depthTexture = createDepthTexture(extent.width, extent.height);
        // m_swapchain->createFramebuffers((VulkanTexture*)m_depthTexture, m_pipeline->getRenderPass());

        // createUniformBuffers();
        // createDescriptorPool();
        // createDescriptorSets();
        // createCommandBuffers();

        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        LOGE("failed to acquire swap chain image!");
    }

    if (m_imagesInFlight[imageIndex])
    {
        m_imagesInFlight[imageIndex]->wait();
    }
    m_imagesInFlight[imageIndex] = m_inFlightFences[m_currentFrame];
    m_swapchain->setCurrentActivedImageIndex(imageIndex);

    // TODO: draw

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkSemaphore waitSemaphores[] = {*m_imageAvailableSemaphores[m_currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    VkCommandBuffer commandBuffer = *m_commandBuffers[imageIndex];
    submitInfo.pCommandBuffers = &commandBuffer;

    VkSemaphore signalSemaphores[] = {*m_renderFinishedSemaphores[m_currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    m_inFlightFences[m_currentFrame]->reset();
    if (vkQueueSubmit(m_logicalDevice->getGraphicsQueue(), 1, &submitInfo, *m_inFlightFences[m_currentFrame]) != VK_SUCCESS)
    {
        LOGE("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {*m_swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(m_logicalDevice->getPresentQueue(), &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_frameBufferResized)
    {
        // m_frameBufferResized = false;
        // m_swapchain->recreateSwapChain();
        //
        // createImageViews();
        // m_pipeline->createRenderPass(m_swapChainImageFormat, VulkanUtils::findDepthFormat(m_context->getPickedPhysicalDevice()));
        // m_pipeline->createGraphicsPipeline(m_swapChainExtent.width, m_swapChainExtent.height);
        //
        // m_depthTexture = createDepthTexture(m_swapChainExtent.width, m_swapChainExtent.height);
        //
        // createFramebuffers();
        // createUniformBuffers();
        // createDescriptorPool();
        // createDescriptorSets();
        // createCommandBuffers();
    }
    else if (result != VK_SUCCESS)
    {
        LOGE("failed to present swap chain image!");
    }

    m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

bool VulkanRenderModule::createCommandBuffers()
{
    for (size_t i = 0; i < m_swapchain->getImageCount(); ++i)
    {
        auto commandBuffer = new VulkanCommandBuffer(*m_logicalDevice, *m_commandPool);
        commandBuffer->create();
        m_commandBuffers.push_back(commandBuffer);
    }

    for (size_t i = 0; i < m_commandBuffers.size(); ++i)
    {
        m_commandBuffers[i]->beginCommand();
        {
            VkRect2D rect{{0, 0}, m_swapchain->getImageExtent()};
            m_commandBuffers[i]->beginRenderPass(m_swapchain->getRenderPass(), m_swapchain->getFramebuffers(i), rect);

            //{
            //    // TODO:
            //    vkCmdBindPipeline(*m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *m_pipeline);
            //    VkDeviceSize offset = 0;
            //    VkBuffer vertexBuffer = *m_vertexBuffer;
            //    vkCmdBindVertexBuffers(*m_commandBuffers[i], 0, 1, &vertexBuffer, &offset);
            //    vkCmdBindIndexBuffer(*m_commandBuffers[i], *m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);
            //    VkDescriptorSet descriptorSet = m_descriptorSets->getDescriptorSet(i);
            //    vkCmdBindDescriptorSets(*m_commandBuffers[i],
            //                            VK_PIPELINE_BIND_POINT_GRAPHICS,
            //                            m_pipeline->getPipelineLayout(),
            //                            0,
            //                            1,
            //                            &descriptorSet,
            //                            0,
            //                            nullptr);
            //    vkCmdDrawIndexed(*m_commandBuffers[i], static_cast<uint32_t>(g_indices.size()), 1, 0, 0, 0);
            //}

            m_commandBuffers[i]->endRenderPass();
        }

        m_commandBuffers[i]->endCommand();
    }

    return true;
}

void VulkanRenderModule::destroyCommandBuffers()
{
    for (auto commandBuffer : m_commandBuffers)
    {
        SAFE_DELETE(commandBuffer);
    }
    m_commandBuffers.clear();
}

bool VulkanRenderModule::createSyncObjects()
{
    m_imagesInFlight.resize(m_swapchain->getImageCount(), nullptr);

    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    {
        auto imageAvailable = new VulkanSemaphore(*m_logicalDevice);
        imageAvailable->create();
        m_imageAvailableSemaphores.push_back(imageAvailable);

        auto renderFinished = new VulkanSemaphore(*m_logicalDevice);
        renderFinished->create();
        m_renderFinishedSemaphores.push_back(renderFinished);

        auto inFlight = new VulkanFence(*m_logicalDevice);
        inFlight->create(true);
        m_inFlightFences.push_back(inFlight);
    }

    return true;
}

void VulkanRenderModule::destroySyncObjects()
{
    for (auto semaphores : m_imageAvailableSemaphores)
    {
        SAFE_DELETE(semaphores);
    }

    for (auto semaphores : m_renderFinishedSemaphores)
    {
        SAFE_DELETE(semaphores);
    }

    for (auto fence : m_inFlightFences)
    {
        SAFE_DELETE(fence);
    }

    m_imageAvailableSemaphores.clear();
    m_renderFinishedSemaphores.clear();
    m_inFlightFences.clear();
    m_imagesInFlight.clear();
}

NS_END
