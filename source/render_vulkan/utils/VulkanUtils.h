/*!
 * \file VulkanUtils.h
 *
 * \author zjhlogo
 * \date 2020/04/11
 *
 *
 */
#pragma once
#include <foundation/BaseTypes.h>
#include <foundation/BaseTypesGlm.h>
#include <vulkan/vulkan.hpp>

NS_BEGIN

class VulkanUtils
{
public:
    static bool checkValidationLayerSupported(const std::vector<vk::LayerProperties>& layers, const std::vector<const char*>& requiredLayers);
    static bool checkExtensionsSupported(const std::vector<vk::ExtensionProperties>& extensions, const std::vector<const char*>& requiredExtensions);
    static const std::vector<const char*>& getRequiredValidationLayers();
    static const std::vector<const char*>& getRequiredInstanceExtensions();
    static const std::vector<const char*>& getRequiredDeviceExtensions();
    static bool getBestFitQueueFamilyPropertyIndex(int& graphicsFamily,
                                                   int& presentFamily,
                                                   const vk::PhysicalDevice& physicalDevice,
                                                   const vk::SurfaceKHR& surface,
                                                   const std::vector<vk::QueueFamilyProperties>& queueFamilyProperties);
    static uint32_t findMemoryType(const vk::PhysicalDeviceMemoryProperties& memProperties, uint32_t typeFilter, vk::MemoryPropertyFlags properties);
    static vk::Format findSupportedFormat(const vk::PhysicalDevice& physicalDevice,
                                          const std::vector<vk::Format>& candidates,
                                          vk::ImageTiling tiling,
                                          vk::FormatFeatureFlags features);
    static vk::Format findDepthFormat(const vk::PhysicalDevice& physicalDevice);
};
NS_END
