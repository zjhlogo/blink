/*!
 * \file VulkanRenderModule.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 *
 */
#pragma once
#include <render_base/RenderModule.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

NS_BEGIN

class VulkanRenderModule : public RenderModule
{
public:
    static const int MAX_FRAMES_IN_FLIGHT = 2;

public:
    VulkanRenderModule();
    virtual ~VulkanRenderModule();

    virtual bool createDevice(const glm::ivec2& deviceSize) override;
    virtual void destroyDevice() override;

    virtual BufferObject* createBufferObject(BufferObject::BufferType bufferType) override { return nullptr; };
    virtual bool destroyBufferObject(BufferObject* bufferObject) override { return false; };

    virtual VertexBuffer* createVertexBuffer(BufferAttributes* attributes) override { return nullptr; };
    virtual bool destroyVertexBuffer(VertexBuffer* vertexBuffer) override { return false; };

    virtual Shader* createShaderFromStock(Shader::StockShaders stockShader, uint32 preprocessDefine) override { return nullptr; };
    virtual Shader* createShaderFromBuffer(const char* vsBuffer, const char* gsBuffer, const char* fsBuffer) override { return nullptr; };
    virtual bool destroyShader(Shader* shader) override { return false; };

    virtual Texture* createTexture2D(const tstring& texFile) override;
    virtual Texture* createDepthTexture(int width, int height) override;
    virtual bool destroyTexture(Texture*& texture) override;

    virtual bool gameLoop() override;

    void drawFrame();

    void setFrameBBufferResized(bool resized) { m_frameBufferResized = resized; };

    bool createBuffer(vk::Buffer& buffer,
                      vk::DeviceMemory& bufferMemory,
                      const vk::DeviceSize& size,
                      vk::BufferUsageFlags usage,
                      vk::MemoryPropertyFlags properties);

    vk::PhysicalDevice& getPhysicalDevice() { return m_physicalDevice; };
    vk::Device& getLogicalDevice() { return m_logicalDevice; };
    vk::CommandPool& getCommandPool() { return m_commandPool; };
    vk::Queue& getGraphicsQueue() { return m_graphicsQueue; };

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

    bool createSwapChain();
    void destroySwapChain();

    bool createImageViews();
    void destroyImageViews();

    bool createRenderPass();
    void destroyRenderPass();

    bool createDescriptorSetLayout();
    void destroyDescriptorSetLayout();

    bool createGraphicsPipeline();
    void destroyGraphicsPipeline();

    bool createFramebuffers();
    void destroyFramebuffers();

    bool createCommandPool();
    void destroyCommandPool();

    bool createVertexBuffer();
    void destroyVertexBuffer();

    bool createIndexBuffer();
    void destroyIndexBuffer();

    bool createUniformBuffers();
    void destroyUniformBuffers();

    bool createDescriptorPool();
    void destroyDescriptorPool();

    bool createDescriptorSets();
    void destroyDescriptorSets();

    bool createCommandBuffers();
    void destroyCommandBuffers();

    bool createSyncObjects();
    void destroySyncObjects();

    bool recreateSwapChain();
    void cleanSwapChain();

    const std::vector<const char*>& getRequiredValidationLayers();
    bool checkValidationLayerSupported(const std::vector<vk::LayerProperties>& layers, const std::vector<const char*>& requiredLayers);

    const std::vector<const char*>& getRequiredInstanceExtensions();
    const std::vector<const char*>& getRequiredDeviceExtensions();
    bool checkExtensionsSupported(const std::vector<vk::ExtensionProperties>& extensions, const std::vector<const char*>& requiredExtensions);

    int getBestFitPhysicalDeviceIndex(const std::vector<vk::PhysicalDevice>& physicalDevices, const vk::SurfaceKHR& surface);
    bool getBestFitQueueFamilyPropertyIndex(int& graphicsFamily,
                                            int& presentFamily,
                                            const vk::PhysicalDevice& physicalDevice,
                                            const vk::SurfaceKHR& surface,
                                            const std::vector<vk::QueueFamilyProperties>& queueFamilies);

    vk::ShaderModule createShaderModule(const std::vector<uint8>& shaderCode);
    bool readFileIntoBuffer(std::vector<uint8>& bufferOut, const std::string& filePath);

    uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties);
    void copyBuffer(vk::Buffer& srcBuffer, vk::Buffer& dstBuffer, vk::DeviceSize& size);
    void updateUniformBuffer(uint32_t currentImage);

    vk::CommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(vk::CommandBuffer commandBuffer);

    vk::ImageView createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags);
    vk::Format findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features);
    vk::Format findDepthFormat();

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
    vk::DescriptorSetLayout m_descriptorSetLayout;
    vk::PipelineLayout m_pipelineLayout;
    vk::Pipeline m_pipeline;

    vk::DescriptorPool m_descriptorPool;
    std::vector<vk::DescriptorSet> m_descriptorSets;

    std::vector<vk::Framebuffer> m_swapChainFramebuffers;
    vk::CommandPool m_commandPool;
    std::vector<vk::CommandBuffer> m_commandBuffers;

    vk::Buffer m_vertexBuffer;
    vk::DeviceMemory m_vertexBufferMemory;

    vk::Buffer m_indexBuffer;
    vk::DeviceMemory m_indexBufferMemory;

    std::vector<vk::Buffer> m_uniformBuffers;
    std::vector<vk::DeviceMemory> m_uniformBuffersMemory;

    std::vector<vk::Semaphore> m_imageAvailableSemaphores;
    std::vector<vk::Semaphore> m_renderFinishedSemaphores;
    std::vector<vk::Fence> m_inFlightFences;
    std::vector<vk::Fence> m_imagesInFlight;
    std::size_t m_currentFrame{};

    bool m_frameBufferResized{};

    Texture* m_texture{};
    Texture* m_depthTexture{};

};

NS_END
