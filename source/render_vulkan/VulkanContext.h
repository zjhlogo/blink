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
#include <vulkan/vulkan.hpp>

NS_BEGIN

class VulkanWindow;

class VulkanContext
{
public:
    VulkanContext();
    ~VulkanContext();

    bool initialize(VulkanWindow* window);
    void terminate();

    const std::vector<vk::PhysicalDevice>& getPhysicalDevices() const { return m_physicalDevices; };
    const vk::PhysicalDevice& getPickedPhysicalDevice() const { return m_pickedPhysicalDevice; };
    const vk::SurfaceKHR& getVkSurface() const { return m_surface; };
    const vk::PhysicalDeviceMemoryProperties& getMemoryProperties() const { return m_memoryProperties; };
    const std::vector<vk::QueueFamilyProperties>& getQueueFamilyProperties() const { return m_queueFamilyProperties; };
    const std::vector<vk::LayerProperties>& getLayerProperties() const { return m_layerProperties; };
    const std::vector<vk::ExtensionProperties>& getExtensionProperties() const { return m_extensionProperties; };

private:
    bool createInstance();
    void destroyInstance();

    bool setupDebugMessenger();
    void destroyDebugMessenger();

    bool createSurface();
    void destroySurface();

    bool pickPhysicalDevice();

    static int getBestFitPhysicalDeviceIndex(const std::vector<vk::PhysicalDevice>& physicalDevices, const vk::SurfaceKHR& surface);

private:
    VulkanWindow* m_window{};

    vk::Instance m_instance;
    vk::DispatchLoaderDynamic m_dispatchLoader;
    vk::DebugUtilsMessengerEXT m_debugMessenger;
    std::vector<vk::PhysicalDevice> m_physicalDevices;

    vk::SurfaceKHR m_surface;

    vk::PhysicalDevice m_pickedPhysicalDevice;
    vk::PhysicalDeviceProperties m_properties;
    vk::PhysicalDeviceFeatures m_features;
    vk::PhysicalDeviceMemoryProperties m_memoryProperties;
    std::vector<vk::QueueFamilyProperties> m_queueFamilyProperties;
    std::vector<vk::LayerProperties> m_layerProperties;
    std::vector<vk::ExtensionProperties> m_extensionProperties;
};
NS_END
