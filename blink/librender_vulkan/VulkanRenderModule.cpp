/*!
 * \file VulkanRenderModule.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#include "VulkanRenderModule.h"
#include <set>

namespace blink
{
    static void mousePositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
    }

    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
    }

    static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
    }

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
    {
//         if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
//         {
//             // Message is important enough to show
//         }
//         std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }

    VulkanRenderModule::VulkanRenderModule()
    {

    }

    VulkanRenderModule::~VulkanRenderModule()
    {

    }

    bool VulkanRenderModule::createDevice(const glm::ivec2& deviceSize)
    {
        /* Initialize the library */
        if (!glfwInit()) return false;

        if (!createWindow(deviceSize)) return false;
        if (!createInstance()) return false;
        if (!setupDebugMessenger()) return false;
        if (!createSurface()) return false;
        if (!pickPhysicalDevice()) return false;
        if (!createLogicalDevice()) return false;

        return true;
    }

    void VulkanRenderModule::destroyDevice()
    {
        destroyLogicalDevice();
        destroySurface();
        destroyDebugMessenger();
        destroyInstance();
        destroyWindow();
        glfwTerminate();
    }

    bool VulkanRenderModule::gameLoop()
    {
        double begin = glfwGetTime();

        /* Loop until the user closes the window */
        if (glfwWindowShouldClose(m_window)) return false;

        /* Poll for and process events */
        glfwPollEvents();

        double end = glfwGetTime();
        double duration = end - begin;
        begin = end;

//         app->step(static_cast<float>(duration));

        /* Swap front and back buffers */
        glfwSwapBuffers(m_window);

        return true;
    }

    bool VulkanRenderModule::createWindow(const glm::ivec2& windowSize)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        /* Create a windowed mode window and its OpenGL context */
        m_window = glfwCreateWindow(windowSize.x, windowSize.y, "blink", nullptr, nullptr);
        if (!m_window) return false;

        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(m_window, mousePositionCallback);
        glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
        glfwSetScrollCallback(m_window, mouseScrollCallback);
        glfwSetKeyCallback(m_window, keyCallback);

        /* Make the window's context current */
        glfwMakeContextCurrent(m_window);

        return true;
    }

    void VulkanRenderModule::destroyWindow()
    {
        glfwDestroyWindow(m_window);
    }

    bool VulkanRenderModule::createInstance()
    {
        vk::ApplicationInfo appInfo;
        appInfo.pApplicationName = "vulkan";
        appInfo.pEngineName = "blink";
        appInfo.apiVersion = VK_API_VERSION_1_0;

        vk::InstanceCreateInfo createInfo;
        createInfo.pApplicationInfo = &appInfo;

        // this is the vulkan supported layers
        std::vector<vk::LayerProperties> layers = vk::enumerateInstanceLayerProperties();
        if (checkValidationLayerSupported(layers, getRequiredValidationLayers()))
        {
            const auto& requiredLayers = getRequiredValidationLayers();
            createInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
            createInfo.ppEnabledLayerNames = requiredLayers.data();
        }

        // this is the vulkan supported extensions
        std::vector<vk::ExtensionProperties> extensions = vk::enumerateInstanceExtensionProperties();
        if (checkExtensionsSupported(extensions, getRequiredInstanceExtensions()))
        {
            const auto& requiredExtensions = getRequiredInstanceExtensions();
            createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
            createInfo.ppEnabledExtensionNames = requiredExtensions.data();
        }

        m_instance = vk::createInstance(createInfo);
        m_dispatchLoader.init(m_instance);

        return true;
    }

    void VulkanRenderModule::destroyInstance()
    {
        m_instance.destroy();
    }

    bool VulkanRenderModule::setupDebugMessenger()
    {
        vk::DebugUtilsMessengerCreateInfoEXT createInfo;
        createInfo.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
        createInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
        createInfo.pfnUserCallback = debugCallback;
        m_debugMessenger = m_instance.createDebugUtilsMessengerEXT(createInfo, nullptr, m_dispatchLoader);

        return true;
    }

    void VulkanRenderModule::destroyDebugMessenger()
    {
        m_instance.destroyDebugUtilsMessengerEXT(m_debugMessenger, nullptr, m_dispatchLoader);
    }

    bool VulkanRenderModule::createSurface()
    {
        VkSurfaceKHR surface;
        if (VK_SUCCESS != glfwCreateWindowSurface((VkInstance)m_instance, m_window, nullptr, &surface))
        {
            return false;
        }
        m_surface = vk::SurfaceKHR(surface);

        return true;
    }

    void VulkanRenderModule::destroySurface()
    {
        m_instance.destroySurfaceKHR(m_surface);
    }

    bool VulkanRenderModule::pickPhysicalDevice()
    {
        std::vector<vk::PhysicalDevice> physicalDevices = m_instance.enumeratePhysicalDevices();
        int index = getBestFitPhysicalDeviceIndex(physicalDevices, m_surface);
        if (index != -1)
        {
            m_physicalDevice = physicalDevices[index];
            return true;
        }

        return false;
    }

    bool VulkanRenderModule::createLogicalDevice()
    {
        // get best fit queue index from queue families
        std::vector<vk::QueueFamilyProperties> queueFamilyProperties = m_physicalDevice.getQueueFamilyProperties();

        int graphicsFamilyIndex{};
        int presentFamilyIndex{};
        if (!getBestFitQueueFamilyPropertyIndex(graphicsFamilyIndex, presentFamilyIndex, m_physicalDevice, m_surface, queueFamilyProperties))
        {
            return false;
        }

        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
        std::set<int> uniqueQueueFamilies = { graphicsFamilyIndex, presentFamilyIndex };

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

        vk::DeviceCreateInfo deviceCreateInfo;
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
        deviceCreateInfo.enabledExtensionCount = 0;

        std::vector<vk::LayerProperties> layers = m_physicalDevice.enumerateDeviceLayerProperties();
        if (checkValidationLayerSupported(layers, getRequiredValidationLayers()))
        {
            const auto& requiredLayers = getRequiredValidationLayers();
            deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
            deviceCreateInfo.ppEnabledLayerNames = requiredLayers.data();
        }

        std::vector<vk::ExtensionProperties> extensions = m_physicalDevice.enumerateDeviceExtensionProperties();
        if (checkExtensionsSupported(extensions, getRequiredDeviceExtensions()))
        {
            const auto& requiredExtensions = getRequiredDeviceExtensions();
            deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
            deviceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();
        }

        m_logicalDevice = m_physicalDevice.createDevice(deviceCreateInfo);

        // retriveing queue handles
        m_graphicsQueue = m_logicalDevice.getQueue(graphicsFamilyIndex, 0);
        m_presentQueue = m_logicalDevice.getQueue(presentFamilyIndex, 0);

        return true;
    }

    void VulkanRenderModule::destroyLogicalDevice()
    {
        // TODO: 
    }

    bool VulkanRenderModule::createSwapchain()
    {
        std::vector<vk::SurfaceFormatKHR> surfaceFormats = m_physicalDevice.getSurfaceFormatsKHR(m_surface);

        // always select first available color format
        vk::Format colorFormat = surfaceFormats[0].format;
        vk::ColorSpaceKHR colorSpace = surfaceFormats[0].colorSpace;

        return false;
    }

    void VulkanRenderModule::destroySwapchain()
    {

    }

    const std::vector<const char*>& VulkanRenderModule::getRequiredValidationLayers()
    {
        static const std::vector<const char*> REQUIRED_LAYERS =
        {
            "VK_LAYER_KHRONOS_validation"
        };

        return REQUIRED_LAYERS;
    }

    bool VulkanRenderModule::checkValidationLayerSupported(const std::vector<vk::LayerProperties>& layers, const std::vector<const char *>& requiredLayers)
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

    const std::vector<const char*>& VulkanRenderModule::getRequiredInstanceExtensions()
    {
        static const std::vector<const char*> REQUIRED_EXTENSIONS =
        {
            "VK_KHR_surface",
            "VK_KHR_win32_surface",
            "VK_EXT_debug_utils",
        };

        return REQUIRED_EXTENSIONS;
    }

    const std::vector<const char*>& VulkanRenderModule::getRequiredDeviceExtensions()
    {
        static const std::vector<const char*> REQUIRED_EXTENSIONS =
        {
            "VK_KHR_swapchain",
        };

        return REQUIRED_EXTENSIONS;
    }

    bool VulkanRenderModule::checkExtensionsSupported(const std::vector<vk::ExtensionProperties>& extensions, const std::vector<const char *>& requiredExtensions)
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

    int VulkanRenderModule::getBestFitPhysicalDeviceIndex(const std::vector<vk::PhysicalDevice>& physicalDevices, const vk::SurfaceKHR& surface)
    {
        int index = 0;
        for (const auto& device : physicalDevices)
        {
            std::vector<vk::QueueFamilyProperties> queueFamilyProperties = device.getQueueFamilyProperties();

            int graphicsFamilyIndex{};
            int presentFamilyIndex{};
            getBestFitQueueFamilyPropertyIndex(graphicsFamilyIndex, presentFamilyIndex, device, surface, queueFamilyProperties);

            auto extensions = device.enumerateDeviceExtensionProperties();
            bool extensionsSupported = checkExtensionsSupported(extensions, getRequiredDeviceExtensions());

            bool swapChainAdequate = false;
            if (extensionsSupported)
            {
                vk::SurfaceCapabilitiesKHR capabilities = device.getSurfaceCapabilitiesKHR(surface);
                std::vector<vk::SurfaceFormatKHR> formats = device.getSurfaceFormatsKHR(surface);
                std::vector<vk::PresentModeKHR> presentModes = device.getSurfacePresentModesKHR(surface);

                swapChainAdequate = !formats.empty() && !presentModes.empty();
            }

            if (graphicsFamilyIndex != -1
                && presentFamilyIndex != -1
                && extensionsSupported
                && swapChainAdequate)
            {
                return index;
            }

            ++index;
        }

        return -1;
    }

    bool VulkanRenderModule::getBestFitQueueFamilyPropertyIndex(int& graphicsFamily, int& presentFamily, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const std::vector<vk::QueueFamilyProperties>& queueFamilies)
    {
        graphicsFamily = -1;
        presentFamily = -1;

        uint32_t i = 0;
        for (const auto& queueFamily : queueFamilies)
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
}
