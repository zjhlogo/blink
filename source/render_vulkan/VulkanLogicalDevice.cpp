/*!
 * \file VulkanLogicalDevice.cpp
 *
 * \author zjhlogo
 * \date 2020/04/11
 *
 *
 */
#include "VulkanLogicalDevice.h"
#include "VulkanCommandBuffer.h"
#include "VulkanCommandPool.h"
#include "VulkanContext.h"
#include "VulkanDescriptorPool.h"
#include "utils/VulkanUtils.h"

#include <set>

namespace blink
{
    VulkanLogicalDevice::VulkanLogicalDevice(VulkanContext* context)
        : m_context(context)
    {
        //
    }

    VulkanLogicalDevice::~VulkanLogicalDevice()
    {
        //
        destroy();
    }

    bool VulkanLogicalDevice::create()
    {
        if (!createLogicalDevice())
        {
            return false;
        }

        m_commandPool = new VulkanCommandPool(*this);
        if (!m_commandPool->create())
        {
            return false;
        }

        m_descriptorPool = new VulkanDescriptorPool(*this);
        if (!m_descriptorPool->create())
        {
            return false;
        }

        return true;
    }

    void VulkanLogicalDevice::destroy()
    {
        SAFE_DELETE(m_descriptorPool);
        SAFE_DELETE(m_commandPool);

        destroyLogicalDevice();
    }

    void VulkanLogicalDevice::waitDeviceIdle()
    {
        if (m_logicalDevice != VK_NULL_HANDLE)
        {
            vkDeviceWaitIdle(m_logicalDevice);
        }
    }

    void VulkanLogicalDevice::waitGraphicsQueueIdle()
    {
        if (m_graphicsQueue != VK_NULL_HANDLE)
        {
            vkQueueWaitIdle(m_graphicsQueue);
        }
    }

    void VulkanLogicalDevice::resetDescriptorPool()
    {
        //
        m_descriptorPool->reset();
    }

    void VulkanLogicalDevice::executeCommand(std::function<void(VulkanCommandBuffer& commandBuffer)>&& cb)
    {
        VulkanCommandBuffer commandBuffer(*this, *m_commandPool);
        commandBuffer.create();
        commandBuffer.beginCommand();

        cb(commandBuffer);

        commandBuffer.endCommand();
        commandBuffer.submitCommand();

        waitGraphicsQueueIdle();
        commandBuffer.destroy();
    }

    bool VulkanLogicalDevice::createLogicalDevice()
    {
        // get best fit queue index from queue families
        if (!VulkanUtils::getBestFitQueueFamilyPropertyIndex(m_graphicsFamilyIndex,
                                                             m_presentFamilyIndex,
                                                             m_context->getPickedPhysicalDevice(),
                                                             m_context->getVkSurface(),
                                                             m_context->getQueueFamilyProperties()))
        {
            return false;
        }

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<int> uniqueQueueFamilies = {m_graphicsFamilyIndex, m_presentFamilyIndex};

        float priority = 1.0f;
        for (uint32_t queueFamilyIndex : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &priority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;
        deviceFeatures.fillModeNonSolid = VK_TRUE;

        VkDeviceCreateInfo deviceCreateInfo{};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
        deviceCreateInfo.enabledExtensionCount = 0;

        if (VulkanUtils::checkValidationLayerSupported(m_context->getLayerProperties(), VulkanUtils::getRequiredValidationLayers()))
        {
            const auto& requiredLayers = VulkanUtils::getRequiredValidationLayers();
            deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
            deviceCreateInfo.ppEnabledLayerNames = requiredLayers.data();
        }

        if (VulkanUtils::checkExtensionsSupported(m_context->getExtensionProperties(), VulkanUtils::getRequiredDeviceExtensions()))
        {
            const auto& requiredExtensions = VulkanUtils::getRequiredDeviceExtensions();
            deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
            deviceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();
        }

        VK_CHECK_RESULT_VOID(vkCreateDevice(m_context->getPickedPhysicalDevice(), &deviceCreateInfo, nullptr, &m_logicalDevice))

        // retrieving queue handles
        vkGetDeviceQueue(m_logicalDevice, m_graphicsFamilyIndex, 0, &m_graphicsQueue);
        vkGetDeviceQueue(m_logicalDevice, m_presentFamilyIndex, 0, &m_presentQueue);

        const auto& properties = m_context->getProperties();
        m_minUniformBufferOffsetAlignment = properties.limits.minUniformBufferOffsetAlignment;

        return true;
    }

    void VulkanLogicalDevice::destroyLogicalDevice()
    {
        m_minUniformBufferOffsetAlignment = 0;

        if (m_logicalDevice != nullptr)
        {
            vkDestroyDevice(m_logicalDevice, nullptr);
            m_logicalDevice = nullptr;
        }
    }
} // namespace blink
