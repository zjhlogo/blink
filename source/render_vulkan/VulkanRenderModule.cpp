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
#include "VulkanUniformBuffer.h"
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
    if (!m_descriptorPool->create()) return false;

    m_commandBuffer = new VulkanCommandBuffer(*m_logicalDevice, *m_commandPool);
    if (!m_commandBuffer->create()) return false;

    m_uniformBuffer = new VulkanUniformBuffer(*m_logicalDevice);
    if (!m_uniformBuffer->create()) return false;

    if (!createSyncObjects()) return false;

    // TODO: setup perframe uniforms inside camera
    m_perFrameUniforms.matWorldToCamera = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    const auto& extent = m_swapchain->getImageExtent();
    m_perFrameUniforms.matCameraToProjection = glm::perspective(glm::radians(45.0f), extent.width / (float)extent.height, 0.1f, 10.0f);
    m_perFrameUniforms.matCameraToProjection[1][1] *= -1.0f;
    m_perFrameUniforms.matWorldToProjection = m_perFrameUniforms.matCameraToProjection * m_perFrameUniforms.matWorldToCamera;

    return true;
}

void VulkanRenderModule::destroyDevice()
{
    waitIdle();

    destroySyncObjects();

    SAFE_DELETE(m_uniformBuffer);
    SAFE_DELETE(m_commandBuffer);
    SAFE_DELETE(m_descriptorPool);
    SAFE_DELETE(m_swapchain);
    SAFE_DELETE(m_commandPool);
    SAFE_DELETE(m_logicalDevice);
    SAFE_DELETE(m_context);
    SAFE_DELETE(m_window);

    glfwTerminate();
}

bool VulkanRenderModule::update()
{
    /* Loop until the user closes the window */
    if (glfwWindowShouldClose(*m_window))
    {
        return false;
    }

    /* Poll for and process events */
    glfwPollEvents();

    return true;
}

void VulkanRenderModule::render(const RenderCb& cb)
{
    // acquire an image and wait for it became ready to use
    uint32_t imageIndex{};
    auto result = vkAcquireNextImageKHR(*m_logicalDevice, *m_swapchain, UINT64_MAX, VK_NULL_HANDLE, *m_acquireImageFence, &imageIndex);
    m_acquireImageFence->wait();
    m_acquireImageFence->reset();

    m_swapchain->setCurrentActivedImageIndex(imageIndex);

    // wait graphics queue idle
    m_logicalDevice->waitGraphicsQueueIdle();

    m_descriptorPool->reset();
    m_uniformBuffer->reset();

    m_uniformBuffer->appendPerFrameBufferData(&m_perFrameUniforms, sizeof(m_perFrameUniforms));

    // record command buffer
    {
        m_commandBuffer->beginCommand();

        {
            VkRect2D rect{{0, 0}, m_swapchain->getImageExtent()};
            m_commandBuffer->beginRenderPass(m_swapchain->getRenderPass(), m_swapchain->getFramebuffers(imageIndex), rect);

            // record commands
            cb(*m_commandBuffer, *m_uniformBuffer, *m_descriptorPool);

            m_commandBuffer->endRenderPass();
        }

        m_commandBuffer->endCommand();
    }

    m_uniformBuffer->flushBuffer();

    // submit command
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = nullptr;
    submitInfo.pWaitDstStageMask = nullptr;
    submitInfo.commandBufferCount = 1;
    VkCommandBuffer commandBuffer = *m_commandBuffer;
    submitInfo.pCommandBuffers = &commandBuffer;

    if (vkQueueSubmit(m_logicalDevice->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
    {
        LOGE("failed to submit draw command buffer!");
    }

    // present queue
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    VkSwapchainKHR swapChains[] = {*m_swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(m_logicalDevice->getPresentQueue(), &presentInfo);
}

void VulkanRenderModule::waitIdle()
{
    if (m_logicalDevice)
    {
        m_logicalDevice->waitGraphicsQueueIdle();
        m_logicalDevice->waitDeviceIdle();
    }
}

bool VulkanRenderModule::createSyncObjects()
{
    m_acquireImageFence = new VulkanFence(*m_logicalDevice);
    m_acquireImageFence->create(false);

    return true;
}

void VulkanRenderModule::destroySyncObjects()
{
    SAFE_DELETE(m_acquireImageFence);
}

NS_END
