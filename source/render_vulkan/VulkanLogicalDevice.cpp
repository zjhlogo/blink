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

#include <set>

NS_BEGIN

VulkanLogicalDevice::VulkanLogicalDevice()
{
}

VulkanLogicalDevice::~VulkanLogicalDevice()
{
}

bool VulkanLogicalDevice::initialize(VulkanContext* context)
{
    m_context = context;

    if (!createLogicalDevice()) return false;
    return true;
}

void VulkanLogicalDevice::terminate()
{
    destroyLogicalDevice();
}

vk::ImageView NS::VulkanLogicalDevice::createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags)
{
    vk::ImageViewCreateInfo viewInfo;
    viewInfo.image = image;
    viewInfo.viewType = vk::ImageViewType::e2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;
    return m_logicalDevice.createImageView(viewInfo);
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

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set<int> uniqueQueueFamilies = {graphicsFamilyIndex, presentFamilyIndex};

    float priority = 1.0f;
    for (uint32_t queueFamilyIndex : uniqueQueueFamilies)
    {
        vk::DeviceQueueCreateInfo queueCreateInfo;
        queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &priority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    vk::PhysicalDeviceFeatures deviceFeatures;
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    vk::DeviceCreateInfo deviceCreateInfo;
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

    m_logicalDevice = m_context->getPickedPhysicalDevice().createDevice(deviceCreateInfo);

    // retriveing queue handles
    m_graphicsQueue = m_logicalDevice.getQueue(graphicsFamilyIndex, 0);
    m_presentQueue = m_logicalDevice.getQueue(presentFamilyIndex, 0);

    return true;
}

void VulkanLogicalDevice::destroyLogicalDevice()
{
    m_logicalDevice.destroy();
}

NS_END
