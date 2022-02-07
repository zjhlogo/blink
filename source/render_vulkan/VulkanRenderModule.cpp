/*!
 * \file VulkanRenderModule.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 *
 */
#include "VulkanRenderModule.h"
#include "VulkanBuffer.h"
#include "VulkanCommandBuffer.h"
#include "VulkanContext.h"
#include "VulkanFence.h"
#include "VulkanLogicalDevice.h"
#include "VulkanSemaphore.h"
#include "VulkanSwapchain.h"
#include "VulkanUniformBuffer.h"
#include "VulkanWindow.h"
#include "utils/VulkanUtils.h"

#include <foundation/Log.h>

namespace blink
{
    VulkanRenderModule::VulkanRenderModule()
    {
        //
    }

    VulkanRenderModule::~VulkanRenderModule()
    {
        //
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

        m_swapchain = new VulkanSwapchain(*m_window, *m_logicalDevice);
        if (!m_swapchain->create()) return false;

        m_commandBuffer = new VulkanCommandBuffer(*m_logicalDevice, m_logicalDevice->getCommandPool());
        if (!m_commandBuffer->create()) return false;

        m_perFrameUniformBuffer = new VulkanUniformBuffer(*m_logicalDevice);
        if (!m_perFrameUniformBuffer->create()) return false;

        m_perMaterialUniformBuffer = new VulkanUniformBuffer(*m_logicalDevice);
        if (!m_perMaterialUniformBuffer->create()) return false;

        m_perInstanceUniformBuffer = new VulkanUniformBuffer(*m_logicalDevice);
        if (!m_perInstanceUniformBuffer->create()) return false;

        if (!createSyncObjects()) return false;

        return true;
    }

    void VulkanRenderModule::destroyDevice()
    {
        waitIdle();

        destroySyncObjects();

        SAFE_DELETE(m_perInstanceUniformBuffer);
        SAFE_DELETE(m_perMaterialUniformBuffer);
        SAFE_DELETE(m_perFrameUniformBuffer);
        SAFE_DELETE(m_commandBuffer);
        SAFE_DELETE(m_swapchain);
        SAFE_DELETE(m_logicalDevice);
        SAFE_DELETE(m_context);
        SAFE_DELETE(m_window);

        glfwTerminate();
    }

    bool VulkanRenderModule::processEvent()
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

    VulkanRenderModule::RenderResult VulkanRenderModule::render(const RenderCb& cb)
    {
        // acquire an image and wait for it became ready to use
        uint32_t imageIndex{};
        auto result = vkAcquireNextImageKHR(*m_logicalDevice, *m_swapchain, UINT64_MAX, VK_NULL_HANDLE, *m_acquireImageFence, &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        {
            // rebuild swap chain
            return RenderResult::Recreate;
        }
        m_acquireImageFence->wait();
        m_acquireImageFence->reset();

        m_swapchain->setCurrentActivedImageIndex(imageIndex);

        // wait graphics queue idle
        m_logicalDevice->waitGraphicsQueueIdle();

        m_logicalDevice->resetDescriptorPool();
        m_perFrameUniformBuffer->reset();
        m_perMaterialUniformBuffer->reset();
        m_perInstanceUniformBuffer->reset();

        // record command buffer
        {
            m_commandBuffer->beginCommand();

            // using negative height to flip the y axis
            const auto& extent = m_swapchain->getImageExtent();
            VkViewport viewport{0.0f, (float)extent.height, (float)extent.width, -(float)extent.height, 0.0f, 1.0f};
            vkCmdSetViewport(*m_commandBuffer, 0, 1, &viewport);

            {
                VkRect2D rect{{0, 0}, extent};
                m_commandBuffer->beginRenderPass(m_swapchain->getRenderPass(), m_swapchain->getFramebuffers(imageIndex), rect);

                // record commands
                cb(*m_commandBuffer, *m_perFrameUniformBuffer, *m_perMaterialUniformBuffer, *m_perInstanceUniformBuffer);

                m_commandBuffer->endRenderPass();
            }

            m_commandBuffer->endCommand();
        }

        m_perFrameUniformBuffer->flushBuffer();
        m_perMaterialUniformBuffer->flushBuffer();
        m_perInstanceUniformBuffer->flushBuffer();

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
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        {
            // rebuild swap chain
            return RenderResult::Recreate;
        }

        return RenderResult::Success;
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
        //
        SAFE_DELETE(m_acquireImageFence);
    }

} // namespace blink
