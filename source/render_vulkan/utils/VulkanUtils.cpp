/*!
 * \file VulkanUtils.cpp
 *
 * \author zjhlogo
 * \date 2020/04/11
 *
 *
 */
#include "VulkanUtils.h"

NS_BEGIN

bool VulkanUtils::checkValidationLayerSupported(const std::vector<vk::LayerProperties>& layers, const std::vector<const char*>& requiredLayers)
{
    for (const auto& requireLayer : requiredLayers)
    {
        bool layerFound = false;
        for (const auto& layer : layers)
        {
            if (std::strcmp(requireLayer, layer.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) return false;
    }
    return true;
}

bool VulkanUtils::checkExtensionsSupported(const std::vector<vk::ExtensionProperties>& extensions, const std::vector<const char*>& requiredExtensions)
{
    for (const auto& requiredExtension : requiredExtensions)
    {
        bool extensionFound = false;
        for (const auto& extension : extensions)
        {
            if (std::strcmp(requiredExtension, extension.extensionName) == 0)
            {
                extensionFound = true;
                break;
            }
        }

        if (!extensionFound) return false;
    }
    return true;
}

const std::vector<const char*>& VulkanUtils::getRequiredValidationLayers()
{
    static const std::vector<const char*> REQUIRED_LAYERS = {"VK_LAYER_KHRONOS_validation"};
    return REQUIRED_LAYERS;
}

const std::vector<const char*>& VulkanUtils::getRequiredInstanceExtensions()
{
    static const std::vector<const char*> REQUIRED_EXTENSIONS = {
        "VK_KHR_surface",
        "VK_KHR_win32_surface",
        "VK_EXT_debug_utils",
    };
    return REQUIRED_EXTENSIONS;
}

const std::vector<const char*>& NS::VulkanUtils::getRequiredDeviceExtensions()
{
    static const std::vector<const char*> REQUIRED_EXTENSIONS = {
        "VK_KHR_swapchain",
    };

    return REQUIRED_EXTENSIONS;
}

bool VulkanUtils::getBestFitQueueFamilyPropertyIndex(int& graphicsFamily,
                                                     int& presentFamily,
                                                     const vk::PhysicalDevice& physicalDevice,
                                                     const vk::SurfaceKHR& surface,
                                                     const std::vector<vk::QueueFamilyProperties>& queueFamilyProperties)
{
    graphicsFamily = -1;
    presentFamily = -1;

    uint32_t i = 0;
    for (const auto& queueFamily : queueFamilyProperties)
    {
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
        {
            graphicsFamily = i;
        }

        if (queueFamily.queueCount > 0 && physicalDevice.getSurfaceSupportKHR(i, surface))
        {
            presentFamily = i;
        }

        if (graphicsFamily != -1 && presentFamily != -1)
        {
            return true;
        }
    }

    return false;
}

uint32_t VulkanUtils::findMemoryType(const vk::PhysicalDeviceMemoryProperties& memProperties, uint32_t typeFilter, vk::MemoryPropertyFlags properties)
{
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
    {
        if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) return i;
    }

    assert(0 && "can not find memory type");
    return -1;
}

vk::Format VulkanUtils::findSupportedFormat(const vk::PhysicalDevice& physicalDevice,
                                            const std::vector<vk::Format>& candidates,
                                            vk::ImageTiling tiling,
                                            vk::FormatFeatureFlags features)
{
    for (vk::Format format : candidates)
    {
        vk::FormatProperties props = physicalDevice.getFormatProperties(format);
        if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features)
        {
            return format;
        }
        else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }

    return vk::Format::eD24UnormS8Uint;
}

vk::Format VulkanUtils::findDepthFormat(const vk::PhysicalDevice& physicalDevice)
{
    return findSupportedFormat(physicalDevice,
                               {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
                               vk::ImageTiling::eOptimal,
                               vk::FormatFeatureFlagBits::eDepthStencilAttachment);
}

NS_END
