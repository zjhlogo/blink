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
#include "VulkanFrameBuffer.h"
#include "VulkanLogicalDevice.h"
#include "VulkanRenderPass.h"
#include "VulkanSwapchain.h"
#include "VulkanUniformBuffer.h"
#include "VulkanWindow.h"
#include "utils/VulkanUtils.h"

#include <core/modules/IResModule.h>
#include <tracy-0.8.2/Tracy.hpp>

#define GLFW_INCLUDE_VULKAN
#include <glfw3/glfw3.h>

namespace blink
{
    IRenderModule* getRenderModule()
    {
        static VulkanRenderModule vulkanRenderModule;
        return &vulkanRenderModule;
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

        m_renderPass = new VulkanRenderPass(*m_logicalDevice);
        if (!m_renderPass->create()) return false;

        m_swapchain = new VulkanSwapchain(*m_window, *m_logicalDevice, *m_renderPass);
        if (!m_swapchain->create()) return false;

        m_commandBuffer = new VulkanCommandBuffer(*m_logicalDevice, m_logicalDevice->getCommandPool());
        if (!m_commandBuffer->create()) return false;

        m_perFrameUniformBuffer = new VulkanUniformBuffer(*m_logicalDevice);
        if (!m_perFrameUniformBuffer->create()) return false;

        m_perMaterialUniformBuffer = new VulkanUniformBuffer(*m_logicalDevice);
        if (!m_perMaterialUniformBuffer->create()) return false;

        if (!createSyncObjects()) return false;

        return true;
    }

    void VulkanRenderModule::destroyDevice()
    {
        waitIdle();

        destroySyncObjects();

        destroyRenderSystems();

        SAFE_DELETE(m_perMaterialUniformBuffer);
        SAFE_DELETE(m_perFrameUniformBuffer);
        SAFE_DELETE(m_commandBuffer);
        SAFE_DELETE(m_swapchain);
        SAFE_DELETE(m_renderPass);
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

    bool VulkanRenderModule::beginRender()
    {
        // acquire an image and wait for it became ready to use
        uint32_t imageIndex{};
        auto result = vkAcquireNextImageKHR(*m_logicalDevice, *m_swapchain, UINT64_MAX, VK_NULL_HANDLE, *m_acquireImageFence, &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        {
            // rebuild swap chain
            m_window->updateFrameBufferSize();
            m_logicalDevice->waitDeviceIdle();
            m_swapchain->recreate();
            return false;
        }
        m_acquireImageFence->wait();
        m_acquireImageFence->reset();

        m_swapchain->setCurrentActiveImageIndex(imageIndex);

        // wait graphics queue idle
        m_logicalDevice->waitGraphicsQueueIdle();

        m_logicalDevice->resetDescriptorPool();
        m_perFrameUniformBuffer->reset();
        m_perMaterialUniformBuffer->reset();

        // start record command buffer
        m_commandBuffer->beginCommand();

        return true;
    }

    void VulkanRenderModule::endRender()
    {
        // end record command buffer
        m_commandBuffer->endCommand();

        m_perMaterialUniformBuffer->flushBuffer();
        m_perFrameUniformBuffer->flushBuffer();

        // submit command
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 0;
        submitInfo.pWaitSemaphores = nullptr;
        submitInfo.pWaitDstStageMask = nullptr;
        submitInfo.commandBufferCount = 1;
        VkCommandBuffer commandBuffer = *m_commandBuffer;
        submitInfo.pCommandBuffers = &commandBuffer;

        VK_CHECK_RESULT(vkQueueSubmit(m_logicalDevice->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE));

        auto imageIndex = m_swapchain->getCurrentActiveImageIndex();

        // present queue
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        VkSwapchainKHR swapChains[] = {*m_swapchain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;

        auto result = vkQueuePresentKHR(m_logicalDevice->getPresentQueue(), &presentInfo);

        FrameMark;

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        {
            // rebuild swap chain
            m_window->updateFrameBufferSize();
            m_logicalDevice->waitDeviceIdle();
            m_swapchain->recreate();
        }
    }

    bool VulkanRenderModule::beginRenderPass()
    {
        // using negative height to flip the y axis
        const auto& extent = m_swapchain->getImageExtent();
        VkViewport viewport{0.0f, static_cast<float>(extent.height), static_cast<float>(extent.width), -static_cast<float>(extent.height), 0.0f, 1.0f};
        vkCmdSetViewport(*m_commandBuffer, 0, 1, &viewport);

        VkRect2D rect{{0, 0}, extent};
        m_commandBuffer->beginRenderPass(*m_renderPass, m_swapchain->getCurrentActiveFrameBuffer(), rect);

        return true;
    }

    void VulkanRenderModule::endRenderPass()
    {
        m_commandBuffer->endRenderPass();
    }

    void VulkanRenderModule::render()
    {
        if (beginRender())
        {
            if (beginRenderPass())
            {
                for (auto sys : m_renderSystems)
                {
                    sys->render();
                }

                endRenderPass();
            }

            endRender();
        }
    }

    void VulkanRenderModule::waitIdle()
    {
        if (m_logicalDevice)
        {
            m_logicalDevice->waitGraphicsQueueIdle();
            m_logicalDevice->waitDeviceIdle();
        }
    }

    glm::vec2 VulkanRenderModule::getSurfaceSize() const
    {
        const auto& ext = m_swapchain->getImageExtent();
        return {ext.width, ext.height};
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
