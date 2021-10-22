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
#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

NS_BEGIN

class VulkanWindow;
class VulkanContext;
class VulkanLogicalDevice;
class VulkanSwapchain;
class VulkanPipeline;
class VulkanBuffer;
class VulkanDescriptorPool;
class VulkanDescriptorSets;
class VulkanCommandPool;
class VulkanCommandBuffer;
class VulkanSemaphore;
class VulkanFence;

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

private:
    bool createCommandBuffers();
    void destroyCommandBuffers();

    bool createSyncObjects();
    void destroySyncObjects();

    void updateUniformBuffer(uint32_t currentImage);

private:
    std::vector<VulkanCommandBuffer*> m_commandBuffers;
    std::vector<VulkanSemaphore*> m_imageAvailableSemaphores;
    std::vector<VulkanSemaphore*> m_renderFinishedSemaphores;
    std::vector<VulkanFence*> m_inFlightFences;
    std::vector<VulkanFence*> m_imagesInFlight;

    std::size_t m_currentFrame{};
    bool m_frameBufferResized{};

    VulkanWindow* m_window{};
    VulkanContext* m_context{};
    VulkanLogicalDevice* m_logicalDevice{};
    VulkanSwapchain* m_swapchain{};
    VulkanPipeline* m_pipeline{};
    VulkanCommandPool* m_commandPool{};

    Texture* m_texture{};
    Texture* m_depthTexture{};
    VulkanBuffer* m_vertexBuffer{};
    VulkanBuffer* m_indexBuffer{};
    std::vector<VulkanBuffer*> m_uniformBuffers;

    VulkanDescriptorPool* m_descriptorPool{};
    VulkanDescriptorSets* m_descriptorSets{};
};

NS_END
