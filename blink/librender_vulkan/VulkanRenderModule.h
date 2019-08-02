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
        VulkanRenderModule();
        virtual ~VulkanRenderModule();

        virtual bool createDevice(const glm::ivec2& deviceSize) override;
        virtual void destroyDevice() override;

        virtual bool gameLoop() override;

        void drawFrame();

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

        bool createImageViews();
        void destroyImageViews();

        bool createRenderPass();
        void destroyRenderPass();

        bool createGraphicsPipeline();
        void destroyGraphicsPipeline();

        bool createFramebuffers();
        void destroyFramebuffers();

        bool createCommandPool();
        void destroyCommandPool();

        bool createCommandBuffers();
        void destroyCommandBuffers();

        bool createSemaphores();
        void destroySemaphores();

        const std::vector<const char*>& getRequiredValidationLayers();
        bool checkValidationLayerSupported(const std::vector<vk::LayerProperties>& layers, const std::vector<const char*>& requiredLayers);

        const std::vector<const char*>& getRequiredInstanceExtensions();
        const std::vector<const char*>& getRequiredDeviceExtensions();
        bool checkExtensionsSupported(const std::vector<vk::ExtensionProperties>& extensions, const std::vector<const char*>& requiredExtensions);

        int getBestFitPhysicalDeviceIndex(const std::vector<vk::PhysicalDevice>& physicalDevices, const vk::SurfaceKHR& surface);
        bool getBestFitQueueFamilyPropertyIndex(int& graphicsFamily, int& presentFamily, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const std::vector<vk::QueueFamilyProperties>& queueFamilies);

        vk::ShaderModule createShaderModule(const std::vector<uint8>& shaderCode);
        bool readFileIntoBuffer(std::vector<uint8>& bufferOut, const std::string& filePath);

    private:
        GLFWwindow* m_window{};
        glm::ivec2 m_deviceSize;

        vk::Instance m_instance;
        vk::DispatchLoaderDynamic m_dispatchLoader;
        vk::DebugUtilsMessengerEXT m_debugMessenger;

        vk::SurfaceKHR m_surface;

        vk::PhysicalDevice m_physicalDevice;
        vk::Device m_logicalDevice;

        vk::Queue m_graphicsQueue;
        vk::Queue m_presentQueue;

        vk::SwapchainKHR m_swapChain;
        std::vector<vk::Image> m_swapChainImages;
        vk::Format m_swapChainImageFormat;
        vk::Extent2D m_swapChainExtent;
        std::vector<vk::ImageView> m_swapChainImageViews;

        vk::RenderPass m_renderPass;
        vk::PipelineLayout m_pipelineLayout;
        vk::Pipeline m_pipeline;

        std::vector<vk::Framebuffer> m_swapChainFramebuffers;
        vk::CommandPool m_commandPool;
        std::vector<vk::CommandBuffer> m_commandBuffers;

        vk::Semaphore m_imageAvailableSemaphore;
        vk::Semaphore m_renderFinishedSemaphore;

    };
}