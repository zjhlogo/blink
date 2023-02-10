/*!
 * \file VulkanUtils.h
 *
 * \author zjhlogo
 * \date 2020/04/11
 *
 *
 */
#pragma once
#include <vulkan/vulkan.h>

#include <vector>

namespace blink
{
    class VulkanUtils
    {
    public:
        static void enumerateInstanceExtensionProperties(std::vector<VkExtensionProperties>& propertiesOut, const char* layerName = nullptr);
        static bool checkExtensionsSupported(const std::vector<VkExtensionProperties>& properties, const std::vector<const char*>& requiredExtensions);

        static void enumerateInstanceLayerProperties(std::vector<VkLayerProperties>& propertiesOut);
        static bool checkValidationLayerSupported(const std::vector<VkLayerProperties>& properties, const std::vector<const char*>& requiredLayers);

        static void enumeratePhysicalDevices(std::vector<VkPhysicalDevice>& devicesOut, VkInstance instance);

        static void getPhysicalDeviceQueueFamilyProperties(std::vector<VkQueueFamilyProperties>& propertiesOut, VkPhysicalDevice device);

        static void getSurfaceFormats(std::vector<VkSurfaceFormatKHR>& formatsOut, VkPhysicalDevice device, VkSurfaceKHR surface);
        static void getSurfacePresentModes(std::vector<VkPresentModeKHR>& presentModesOut, VkPhysicalDevice device, VkSurfaceKHR surface);

        static void enumerateDeviceExtensionProperties(std::vector<VkExtensionProperties>& propertiesOut,
                                                       VkPhysicalDevice device,
                                                       const char* layerName = nullptr);
        static void enumerateDeviceLayerProperties(std::vector<VkLayerProperties>& propertiesOut, VkPhysicalDevice device);

        static VkResult createDebugUtilsMessengerExt(VkInstance instance,
                                                     const VkDebugUtilsMessengerCreateInfoEXT* createInfo,
                                                     const VkAllocationCallbacks* allocator,
                                                     VkDebugUtilsMessengerEXT* debugMessenger);
        static void destroyDebugUtilsMessengerExt(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* allocator);

        static const std::vector<const char*>& getRequiredValidationLayers();
        static const std::vector<const char*>& getRequiredInstanceExtensions();
        static const std::vector<const char*>& getRequiredDeviceExtensions();

        static bool getBestFitQueueFamilyPropertyIndex(int& graphicsFamilyOut,
                                                       int& presentFamilyOut,
                                                       VkPhysicalDevice physicalDevice,
                                                       VkSurfaceKHR surface,
                                                       const std::vector<VkQueueFamilyProperties>& queueFamilyProperties);

        static uint32_t findMemoryType(const VkPhysicalDeviceMemoryProperties& memProperties, uint32_t typeFilter, VkMemoryPropertyFlags properties);

        static VkFormat findSupportedFormat(VkPhysicalDevice physicalDevice,
                                            const std::vector<VkFormat>& candidates,
                                            VkImageTiling tiling,
                                            VkFormatFeatureFlags features);

        static VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);
    };
} // namespace blink
