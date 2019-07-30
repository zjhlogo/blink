/*!
 * \file VulkanRenderModule.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#include "VulkanRenderModule.h"

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

        createWindow(deviceSize);
        createContext();
        createSurface();

        return true;
    }

    void VulkanRenderModule::destroyDevice()
    {
        destroySurface();
        destroyContext();
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

    bool VulkanRenderModule::createContext()
    {
        // create vulkan instance
        {
            vk::ApplicationInfo appInfo;
            appInfo.pApplicationName = "vulkan";
            appInfo.pEngineName = "blink";
            appInfo.apiVersion = VK_API_VERSION_1_0;

            vk::InstanceCreateInfo createInfo;
            createInfo.pApplicationInfo = &appInfo;

            if (checkValidationLayerSupported())
            {
                const auto& layers = getRequiredValidationLayers();
                createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
                createInfo.ppEnabledLayerNames = layers.data();
            }

            if (checkExtensionsSupported())
            {
                const auto& extensions = getRequiredExtensions();
                createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
                createInfo.ppEnabledExtensionNames = extensions.data();
            }

            m_instance = vk::createInstance(createInfo);
        }

        // setup debug message callback
        {
            vk::DebugUtilsMessengerCreateInfoEXT createInfo;
            createInfo.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
            createInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
            createInfo.pfnUserCallback = debugCallback;
            m_debugMessenger = m_instance.createDebugUtilsMessengerEXT(createInfo);
        }

        // get best fit physical device from list
        {
            std::vector<vk::PhysicalDevice> physicalDevices = m_instance.enumeratePhysicalDevices();
            auto index = getBestFitPhysicalDeviceIndex(physicalDevices);
            m_physicalDevice = physicalDevices[index];
        }

        // create a logical device
        {
            // get best fit queue index from queue families
            std::vector<vk::QueueFamilyProperties> queueFamilyProperties = m_physicalDevice.getQueueFamilyProperties();
            auto queueFamilyIndex = getBestFitQueueFamilyPropertyIndex(queueFamilyProperties);

            vk::DeviceQueueCreateInfo queueCreateInfo;
            queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
            queueCreateInfo.queueCount = 1;
            float priority = 1.0f;
            queueCreateInfo.pQueuePriorities = &priority;

            vk::PhysicalDeviceFeatures deviceFeatures;

            vk::DeviceCreateInfo deviceCreateInfo;
            deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
            deviceCreateInfo.queueCreateInfoCount = 1;
            deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
            deviceCreateInfo.enabledExtensionCount = 0;

            const auto& layers = getRequiredValidationLayers();
            deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
            deviceCreateInfo.ppEnabledLayerNames = layers.data();

            m_logicalDevice = m_physicalDevice.createDevice(deviceCreateInfo);

            // retriveing queue handles
            m_queue = m_logicalDevice.getQueue(queueFamilyIndex, 0);
        }

        return true;
    }

    void VulkanRenderModule::destroyContext()
    {
        // TODO: destroy logical device
        m_instance.destroyDebugUtilsMessengerEXT(m_debugMessenger);
        m_instance.destroy();
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

    bool VulkanRenderModule::checkValidationLayerSupported()
    {
        // this is the vulkan supported layers
        std::vector<vk::LayerProperties> layers = vk::enumerateInstanceLayerProperties();

        const auto& requiredLayers = getRequiredValidationLayers();
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

    const std::vector<const char*>& VulkanRenderModule::getRequiredExtensions()
    {
        static const std::vector<const char*> REQUIRED_EXTENSIONS =
        {
            "VK_KHR_surface",
            "VK_KHR_win32_surface",
            "VK_EXT_debug_utils",
        };

        return REQUIRED_EXTENSIONS;
    }

    bool VulkanRenderModule::checkExtensionsSupported()
    {
        // this is the vulkan supported extensions
        std::vector<vk::ExtensionProperties> extensions = vk::enumerateInstanceExtensionProperties();

        const auto& requiredExtensions = getRequiredExtensions();
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

    uint32_t VulkanRenderModule::getBestFitPhysicalDeviceIndex(const std::vector<vk::PhysicalDevice>& physicalDevices)
    {
        return 0;
    }

    uint32_t VulkanRenderModule::getBestFitQueueFamilyPropertyIndex(const std::vector<vk::QueueFamilyProperties>& queueFamilies)
    {
        return 0;
    }
}
