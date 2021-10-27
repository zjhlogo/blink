/*!
 * \file VulkanLogicalDevice.cpp
 *
 * \author zjhlogo
 * \date 2020/04/11
 *
 *
 */
#include "VulkanLogicalDevice.h"
#include "VulkanContext.h"
#include "utils/VulkanUtils.h"

#include <foundation/Log.h>

#include <set>

NS_BEGIN

VulkanLogicalDevice::VulkanLogicalDevice(VulkanContext* context)
    : m_context(context)
{
}

VulkanLogicalDevice::~VulkanLogicalDevice()
{
    destroyLogicalDevice();
}

bool VulkanLogicalDevice::create()
{
    if (!createLogicalDevice()) return false;
    return true;
}

void VulkanLogicalDevice::destroy()
{
    destroyLogicalDevice();
}

void VulkanLogicalDevice::waitDeviceIdle()
{
    vkDeviceWaitIdle(m_logicalDevice);
}

void VulkanLogicalDevice::waitGraphicsQueueIdle()
{
    vkQueueWaitIdle(m_graphicsQueue);
}

bool VulkanLogicalDevice::createLogicalDevice()
{
    // get best fit queue index from queue families
    int graphicsFamilyIndex{};
    int presentFamilyIndex{};
    if (!VulkanUtils::getBestFitQueueFamilyPropertyIndex(graphicsFamilyIndex,
                                                         presentFamilyIndex,
                                                         m_context->getPickedPhysicalDevice(),
                                                         m_context->getVkSurface(),
                                                         m_context->getQueueFamilyProperties()))
    {
        return false;
    }

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<int> uniqueQueueFamilies = {graphicsFamilyIndex, presentFamilyIndex};

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

    if (vkCreateDevice(m_context->getPickedPhysicalDevice(), &deviceCreateInfo, nullptr, &m_logicalDevice) != VK_SUCCESS)
    {
        LOGE("create device failed");
        return false;
    }

    // retriveing queue handles
    vkGetDeviceQueue(m_logicalDevice, graphicsFamilyIndex, 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_logicalDevice, presentFamilyIndex, 0, &m_presentQueue);

    return true;
}

void VulkanLogicalDevice::destroyLogicalDevice()
{
    if (m_logicalDevice != nullptr)
    {
        vkDestroyDevice(m_logicalDevice, nullptr);
        m_logicalDevice = nullptr;
    }
}

NS_END
