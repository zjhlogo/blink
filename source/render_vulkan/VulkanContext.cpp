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

bool VulkanContext::createInstance()
{
    vk::ApplicationInfo appInfo;
    appInfo.pApplicationName = "vulkan";
    appInfo.pEngineName = "blink";
    appInfo.apiVersion = VK_API_VERSION_1_0;

    vk::InstanceCreateInfo createInfo;
    createInfo.pApplicationInfo = &appInfo;

    // this is the vulkan supported layers
    std::vector<vk::LayerProperties> layers = vk::enumerateInstanceLayerProperties();
    if (VulkanUtils::checkValidationLayerSupported(layers, VulkanUtils::getRequiredValidationLayers()))
    {
        const auto& requiredLayers = VulkanUtils::getRequiredValidationLayers();
        createInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
        createInfo.ppEnabledLayerNames = requiredLayers.data();
    }

    // this is the vulkan supported extensions
    std::vector<vk::ExtensionProperties> extensions = vk::enumerateInstanceExtensionProperties();
    if (VulkanUtils::checkExtensionsSupported(extensions, VulkanUtils::getRequiredInstanceExtensions()))
    {
        const auto& requiredExtensions = VulkanUtils::getRequiredInstanceExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();
    }

    m_instance = vk::createInstance(createInfo);
    m_dispatchLoader.init(m_instance);
    m_physicalDevices = m_instance.enumeratePhysicalDevices();

    return true;
}

void VulkanContext::destroyInstance()
{
    m_instance.destroy();
}

bool VulkanContext::setupDebugMessenger()
{
    vk::DebugUtilsMessengerCreateInfoEXT createInfo;
    createInfo.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning
                                 | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
    createInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
                             | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
    createInfo.pfnUserCallback = debugCallback;
    m_debugMessenger = m_instance.createDebugUtilsMessengerEXT(createInfo, nullptr, m_dispatchLoader);

    return true;
}

void VulkanContext::destroyDebugMessenger()
{
    m_instance.destroyDebugUtilsMessengerEXT(m_debugMessenger, nullptr, m_dispatchLoader);
}

bool VulkanContext::createSurface()
{
    VkSurfaceKHR surface;
    if (VK_SUCCESS != glfwCreateWindowSurface((VkInstance)m_instance, m_window->m_window, nullptr, &surface))
    {
        return false;
    }
    m_surface = vk::SurfaceKHR(surface);

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

        m_properties = m_pickedPhysicalDevice.getProperties();
        m_features = m_pickedPhysicalDevice.getFeatures();
        m_memoryProperties = m_pickedPhysicalDevice.getMemoryProperties();
        m_queueFamilyProperties = m_pickedPhysicalDevice.getQueueFamilyProperties();
        m_layerProperties = m_pickedPhysicalDevice.enumerateDeviceLayerProperties();
        m_extensionProperties = m_pickedPhysicalDevice.enumerateDeviceExtensionProperties();

        return true;
    }

    return false;
}

int VulkanContext::getBestFitPhysicalDeviceIndex(const std::vector<vk::PhysicalDevice>& physicalDevices, const vk::SurfaceKHR& surface)
{
    int index = 0;
    for (const auto& device : physicalDevices)
    {
        std::vector<vk::QueueFamilyProperties> queueFamilyProperties = device.getQueueFamilyProperties();

        int graphicsFamilyIndex{};
        int presentFamilyIndex{};
        VulkanUtils::getBestFitQueueFamilyPropertyIndex(graphicsFamilyIndex, presentFamilyIndex, device, surface, queueFamilyProperties);

        auto extensions = device.enumerateDeviceExtensionProperties();
        bool extensionsSupported = VulkanUtils::checkExtensionsSupported(extensions, VulkanUtils::getRequiredDeviceExtensions());
        vk::PhysicalDeviceFeatures supportedFeatures = device.getFeatures();

        bool swapChainAdequate = false;
        if (extensionsSupported)
        {
            vk::SurfaceCapabilitiesKHR capabilities = device.getSurfaceCapabilitiesKHR(surface);
            std::vector<vk::SurfaceFormatKHR> formats = device.getSurfaceFormatsKHR(surface);
            std::vector<vk::PresentModeKHR> presentModes = device.getSurfacePresentModesKHR(surface);

            swapChainAdequate = !formats.empty() && !presentModes.empty();
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
