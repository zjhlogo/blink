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

class VulkanWindow;
class VulkanContext;
class VulkanLogicalDevice;
class VulkanSwapchain;
class VulkanPipeline;

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
    virtual Shader* createShaderFromBuffer(const char* vsBuffer, const char* gsBuffer, const char* fsBuffer) override;
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

    vk::CommandPool& getCommandPool() { return m_commandPool; };

private:
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

    void copyBuffer(vk::Buffer& srcBuffer, vk::Buffer& dstBuffer, vk::DeviceSize& size);
    void updateUniformBuffer(uint32_t currentImage);

    vk::CommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(vk::CommandBuffer commandBuffer);

private:
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

    VulkanWindow* m_window{};
    VulkanContext* m_context{};
    VulkanLogicalDevice* m_logicalDevice{};
    VulkanSwapchain* m_swapchain{};

    Texture* m_texture{};
    Texture* m_depthTexture{};
    VulkanPipeline* m_pipeline{};
};

NS_END
