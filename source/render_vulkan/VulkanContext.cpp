/*!
 * \file VulkanContext.cpp
 *
 * \author zjhlogo
 * \date 2020/04/11
 *
 *
 */
#include "VulkanContext.h"
#include "VulkanWindow.h"
#include "foundation/Log.h"
#include "utils/VulkanUtils.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

NS_BEGIN

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                    void* pUserData)
{
    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
        // TODO: Message is important enough to show
    }

    tstring strError = pCallbackData->pMessage;
    return VK_FALSE;
}

VulkanContext::VulkanContext()
{
}

VulkanContext::~VulkanContext()
{
}

bool VulkanContext::initialize(VulkanWindow* window)
{
    m_window = window;

    if (!createInstance()) return false;
    if (!setupDebugMessenger()) return false;
    if (!createSurface()) return false;
    if (!pickPhysicalDevice()) return false;

    return true;
}

void VulkanContext::terminate()
{
    destroySurface();
    destroyDebugMessenger();
    destroyInstance();
}

uint32_t NS::VulkanContext::findMemoryType(VkMemoryPropertyFlags typeFilter, VkMemoryPropertyFlags properties)
{
    return VulkanUtils::findMemoryType(m_memoryProperties, typeFilter, properties);
}

bool VulkanContext::createInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "vulkan";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "blink";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // this is the vulkan supported layers
    std::vector<VkLayerProperties> layers;
    VulkanUtils::enumerateInstanceLayerProperties(layers);
    if (VulkanUtils::checkValidationLayerSupported(layers, VulkanUtils::getRequiredValidationLayers()))
    {
        const auto& requiredLayers = VulkanUtils::getRequiredValidationLayers();
        createInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
        createInfo.ppEnabledLayerNames = requiredLayers.data();
    }

    // this is the vulkan supported extensions
    std::vector<VkExtensionProperties> extensions;
    VulkanUtils::enumerateInstanceExtensionProperties(extensions);

    if (VulkanUtils::checkExtensionsSupported(extensions, VulkanUtils::getRequiredInstanceExtensions()))
    {
        const auto& requiredExtensions = VulkanUtils::getRequiredInstanceExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();
    }

    if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
    {
        LOGE("create instance failed");
        return false;
    }

    VulkanUtils::enumeratePhysicalDevices(m_physicalDevices, m_instance);

    return true;
}

void VulkanContext::destroyInstance()
{
    vkDestroyInstance(m_instance, nullptr);
}

bool VulkanContext::setupDebugMessenger()
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                                 | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                             | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;

    if (VulkanUtils::createDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS)
    {
        LOGE("create debug messenger failed");
    }

    return true;
}

void VulkanContext::destroyDebugMessenger()
{
    VulkanUtils::destroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
}

bool VulkanContext::createSurface()
{
    if (VK_SUCCESS != glfwCreateWindowSurface(m_instance, m_window->m_window, nullptr, &m_surface))
    {
        LOGE("create window surface failed");
        return false;
    }

    return true;
}

void VulkanContext::destroySurface()
{
}

bool VulkanContext::pickPhysicalDevice()
{
    int index = getBestFitPhysicalDeviceIndex(m_physicalDevices, m_surface);
    if (index != -1)
    {
        m_pickedPhysicalDevice = m_physicalDevices[index];

        vkGetPhysicalDeviceProperties(m_pickedPhysicalDevice, &m_properties);
        vkGetPhysicalDeviceFeatures(m_pickedPhysicalDevice, &m_features);
        vkGetPhysicalDeviceMemoryProperties(m_pickedPhysicalDevice, &m_memoryProperties);
        VulkanUtils::getPhysicalDeviceQueueFamilyProperties(m_queueFamilyProperties, m_pickedPhysicalDevice);
        VulkanUtils::enumerateDeviceExtensionProperties(m_extensionProperties, m_pickedPhysicalDevice);
        VulkanUtils::enumerateDeviceLayerProperties(m_layerProperties, m_pickedPhysicalDevice);

        return true;
    }

    return false;
}

int VulkanContext::getBestFitPhysicalDeviceIndex(const std::vector<VkPhysicalDevice>& physicalDevices, VkSurfaceKHR surface)
{
    int index = 0;
    for (auto device : physicalDevices)
    {
        std::vector<VkQueueFamilyProperties> queueFamilyProperties;
        VulkanUtils::getPhysicalDeviceQueueFamilyProperties(queueFamilyProperties, device);

        int graphicsFamilyIndex{};
        int presentFamilyIndex{};
        VulkanUtils::getBestFitQueueFamilyPropertyIndex(graphicsFamilyIndex, presentFamilyIndex, device, surface, queueFamilyProperties);

        std::vector<VkExtensionProperties> extensions;
        VulkanUtils::enumerateDeviceExtensionProperties(extensions, device);

        bool extensionsSupported = VulkanUtils::checkExtensionsSupported(extensions, VulkanUtils::getRequiredDeviceExtensions());

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

        bool swapChainAdequate = false;
        if (extensionsSupported)
        {
            VkSurfaceCapabilitiesKHR capabilities;
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities);

            uint32_t surfaceFormatCount = 0;
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &surfaceFormatCount, nullptr);

            uint32_t presentModeCount = 0;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

            swapChainAdequate = (surfaceFormatCount > 0) && (presentModeCount > 0);
        }

        if (graphicsFamilyIndex != -1 && presentFamilyIndex != -1 && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy)
        {
            return index;
        }

        ++index;
    }

    return -1;
}

NS_END
