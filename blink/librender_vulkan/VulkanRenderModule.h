/*!
 * \file VulkanRenderModule.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#pragma once
#include <RenderModule.h>

#define GLFW_INCLUDE_VULKAN
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

namespace blink
{
    class VulkanRenderModule : public RenderModule
    {
    public:
        class SwapChainImage
        {
        public:
            vk::Image image;
            vk::ImageView view;
            vk::Fence fence;
        };

    public:
        VulkanRenderModule();
        virtual ~VulkanRenderModule();

        virtual bool createDevice(const glm::ivec2& deviceSize) override;
        virtual void destroyDevice() override;

        virtual bool gameLoop() override;

    private:
        bool createWindow(const glm::ivec2& windowSize);
        void destroyWindow();

        bool createInstance();
        void destroyInstance();

        bool setupDebugMessenger();
        void destroyDebugMessenger();

        bool createSurface();
        void destroySurface();

        bool pickPhysicalDevice();

        bool createLogicalDevice();
        void destroyLogicalDevice();

        bool createSwapchain();
        void destroySwapchain();

        const std::vector<const char*>& getRequiredValidationLayers();
        bool checkValidationLayerSupported(const std::vector<vk::LayerProperties>& layers);

        const std::vector<const char*>& getRequiredExtensions();
        bool checkExtensionsSupported(const std::vector<vk::ExtensionProperties>& extensions);

        int getBestFitPhysicalDeviceIndex(const std::vector<vk::PhysicalDevice>& physicalDevices);
        bool getBestFitQueueFamilyPropertyIndex(int& graphicsFamily, int& presentFamily, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const std::vector<vk::QueueFamilyProperties>& queueFamilies);

    private:
        GLFWwindow* m_window{};

        vk::Instance m_instance;
        vk::DispatchLoaderDynamic m_dispatchLoader;
        vk::DebugUtilsMessengerEXT m_debugMessenger;
        vk::PhysicalDevice m_physicalDevice;
        vk::Device m_logicalDevice;
        vk::Queue m_graphicsQueue;
        vk::Queue m_presentQueue;

        vk::SurfaceKHR m_surface;

        vk::SwapchainKHR m_swapchain;
        std::vector<SwapChainImage> m_images;
        vk::PresentInfoKHR m_presentInfo;

    };
}