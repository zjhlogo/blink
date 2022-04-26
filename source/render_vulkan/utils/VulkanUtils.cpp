/*!
 * \file VulkanUtils.cpp
 *
 * \author zjhlogo
 * \date 2020/04/11
 *
 *
 */
#include "VulkanUtils.h"

#include <foundation/Log.h>

namespace blink
{
    void VulkanUtils::enumerateInstanceExtensionProperties(std::vector<VkExtensionProperties>& propertiesOut,
                                                           const char* layerName /*= nullptr*/)
    {
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, nullptr);
        propertiesOut.resize(extensionCount);
        vkEnumerateInstanceExtensionProperties(layerName, &extensionCount, propertiesOut.data());
    }

    bool VulkanUtils::checkExtensionsSupported(const std::vector<VkExtensionProperties>& properties,
                                               const std::vector<const char*>& requiredExtensions)
    {
        for (const auto& requiredExtension : requiredExtensions)
        {
            bool extensionFound = false;
            for (const auto& extension : properties)
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

    void VulkanUtils::enumerateInstanceLayerProperties(std::vector<VkLayerProperties>& propertiesOut)
    {
        uint32_t propertyCount = 0;
        vkEnumerateInstanceLayerProperties(&propertyCount, nullptr);
        propertiesOut.resize(propertyCount);
        vkEnumerateInstanceLayerProperties(&propertyCount, propertiesOut.data());
    }

    bool VulkanUtils::checkValidationLayerSupported(const std::vector<VkLayerProperties>& properties,
                                                    const std::vector<const char*>& requiredLayers)
    {
        for (const auto& requireLayer : requiredLayers)
        {
            bool layerFound = false;
            for (const auto& layer : properties)
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

    void VulkanUtils::enumeratePhysicalDevices(std::vector<VkPhysicalDevice>& devicesOut, VkInstance instance)
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        devicesOut.resize(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devicesOut.data());
    }

    void VulkanUtils::getPhysicalDeviceQueueFamilyProperties(std::vector<VkQueueFamilyProperties>& propertiesOut,
                                                             VkPhysicalDevice device)
    {
        uint32_t propertyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &propertyCount, nullptr);
        propertiesOut.resize(propertyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &propertyCount, propertiesOut.data());
    }

    void VulkanUtils::getSurfaceFormats(std::vector<VkSurfaceFormatKHR>& formatsOut,
                                        VkPhysicalDevice device,
                                        VkSurfaceKHR surface)
    {
        uint32 formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
        formatsOut.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, formatsOut.data());
    }

    void VulkanUtils::getSurfacePresentModes(std::vector<VkPresentModeKHR>& presentModesOut,
                                             VkPhysicalDevice device,
                                             VkSurfaceKHR surface)
    {
        uint32_t presentCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentCount, nullptr);
        presentModesOut.resize(presentCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentCount, presentModesOut.data());
    }

    void VulkanUtils::enumerateDeviceExtensionProperties(std::vector<VkExtensionProperties>& propertiesOut,
                                                         VkPhysicalDevice device,
                                                         const char* layerName /* = nullptr */)
    {
        uint32_t propertyCount = 0;
        vkEnumerateDeviceExtensionProperties(device, layerName, &propertyCount, nullptr);
        propertiesOut.resize(propertyCount);
        vkEnumerateDeviceExtensionProperties(device, layerName, &propertyCount, propertiesOut.data());
    }

    void VulkanUtils::enumerateDeviceLayerProperties(std::vector<VkLayerProperties>& propertiesOut, VkPhysicalDevice device)
    {
        uint32_t propertyCount = 0;
        vkEnumerateDeviceLayerProperties(device, &propertyCount, nullptr);
        propertiesOut.resize(propertyCount);
        vkEnumerateDeviceLayerProperties(device, &propertyCount, propertiesOut.data());
    }

    VkResult VulkanUtils::createDebugUtilsMessengerEXT(VkInstance instance,
                                                       const VkDebugUtilsMessengerCreateInfoEXT* createInfo,
                                                       const VkAllocationCallbacks* allocator,
                                                       VkDebugUtilsMessengerEXT* debugMessenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            return func(instance, createInfo, allocator, debugMessenger);
        }
        else
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void VulkanUtils::destroyDebugUtilsMessengerEXT(VkInstance instance,
                                                    VkDebugUtilsMessengerEXT debugMessenger,
                                                    const VkAllocationCallbacks* allocator)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            func(instance, debugMessenger, allocator);
        }
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

    const std::vector<const char*>& VulkanUtils::getRequiredDeviceExtensions()
    {
        static const std::vector<const char*> REQUIRED_EXTENSIONS = {
            "VK_KHR_swapchain",
            "VK_KHR_maintenance1",
        };

        return REQUIRED_EXTENSIONS;
    }

    bool VulkanUtils::getBestFitQueueFamilyPropertyIndex(int& graphicsFamily,
                                                         int& presentFamily,
                                                         VkPhysicalDevice physicalDevice,
                                                         VkSurfaceKHR surface,
                                                         const std::vector<VkQueueFamilyProperties>& queueFamilyProperties)
    {
        graphicsFamily = -1;
        presentFamily = -1;

        uint32_t i = 0;
        for (const auto& queueFamily : queueFamilyProperties)
        {
            if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                graphicsFamily = i;
            }

            VkBool32 support = VK_FALSE;
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &support);

            if (queueFamily.queueCount > 0 && support)
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

    uint32_t VulkanUtils::findMemoryType(const VkPhysicalDeviceMemoryProperties& memProperties,
                                         uint32_t typeFilter,
                                         VkMemoryPropertyFlags properties)
    {
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
        {
            if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) return i;
        }

        LOGE("can not find memory type");
        return -1;

        // auto physicalDevice = m_context->getPickedPhysicalDevice();

        // VkPhysicalDeviceMemoryProperties memProperties;
        // vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

        // for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
        //{
        //    if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) return i;
        //}

        // LOGE("can not found memory type {0} {1}", typeFilter, properties);
        // return 0;
    }

    VkFormat VulkanUtils::findSupportedFormat(VkPhysicalDevice physicalDevice,
                                              const std::vector<VkFormat>& candidates,
                                              VkImageTiling tiling,
                                              VkFormatFeatureFlags features)
    {
        for (VkFormat format : candidates)
        {
            VkFormatProperties props{};
            vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
            {
                return format;
            }
            else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
            {
                return format;
            }
        }

        return VK_FORMAT_D24_UNORM_S8_UINT;
    }

    VkFormat VulkanUtils::findDepthFormat(VkPhysicalDevice physicalDevice)
    {
        return findSupportedFormat(physicalDevice,
                                   {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                                   VK_IMAGE_TILING_OPTIMAL,
                                   VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }

} // namespace blink
