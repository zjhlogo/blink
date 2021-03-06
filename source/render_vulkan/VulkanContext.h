/*!
 * \file VulkanContext.h
 *
 * \author zjhlogo
 * \date 2020/04/11
 *
 *
 */
#pragma once
#include <foundation/BaseTypes.h>
#include <foundation/BaseTypesGlm.h>
#include <vulkan/vulkan.h>

#include <vector>

NS_BEGIN

class VulkanWindow;

class VulkanContext
{
public:
    VulkanContext(VulkanWindow* window);
    ~VulkanContext();

    bool create();
    void destroy();

    uint32_t findMemoryType(VkMemoryPropertyFlags typeFilter, VkMemoryPropertyFlags properties);

    VulkanWindow* getWindow() { return m_window; };
    const std::vector<VkPhysicalDevice>& getPhysicalDevices() const { return m_physicalDevices; };
    VkPhysicalDevice getPickedPhysicalDevice() const { return m_pickedPhysicalDevice; };
    VkSurfaceKHR getVkSurface() const { return m_surface; };
    VkPhysicalDeviceMemoryProperties getMemoryProperties() const { return m_memoryProperties; };
    const std::vector<VkQueueFamilyProperties>& getQueueFamilyProperties() const { return m_queueFamilyProperties; };
    const std::vector<VkLayerProperties>& getLayerProperties() const { return m_layerProperties; };
    const std::vector<VkExtensionProperties>& getExtensionProperties() const { return m_extensionProperties; };

private:
    bool createInstance();
    void destroyInstance();

    bool setupDebugMessenger();
    void destroyDebugMessenger();

    bool createSurface();
    void destroySurface();

    bool pickPhysicalDevice();

    static int getBestFitPhysicalDeviceIndex(const std::vector<VkPhysicalDevice>& physicalDevices, VkSurfaceKHR surface);

private:
    VulkanWindow* m_window{};

    VkInstance m_instance;
    VkDebugUtilsMessengerEXT m_debugMessenger;
    std::vector<VkPhysicalDevice> m_physicalDevices;

    VkSurfaceKHR m_surface;

    VkPhysicalDevice m_pickedPhysicalDevice;
    VkPhysicalDeviceProperties m_properties;
    VkPhysicalDeviceFeatures m_features;
    VkPhysicalDeviceMemoryProperties m_memoryProperties;
    std::vector<VkQueueFamilyProperties> m_queueFamilyProperties;
    std::vector<VkLayerProperties> m_layerProperties;
    std::vector<VkExtensionProperties> m_extensionProperties;
};
NS_END
