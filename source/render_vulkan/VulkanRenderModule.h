/*!
 * \file VulkanRenderModule.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 *
 */
#pragma once
#include <foundation/BaseTypesGlm.h>
#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <glfw3/glfw3.h>

#include <vector>

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

class VulkanRenderModule
{
public:
    static const int MAX_FRAMES_IN_FLIGHT = 2;

public:
    VulkanRenderModule();
    virtual ~VulkanRenderModule();

    bool createDevice(const glm::ivec2& deviceSize);
    void destroyDevice();

    bool gameLoop();

    void drawFrame();

    void setFrameBufferResized(bool resized) { m_frameBufferResized = resized; };

private:
    bool createCommandBuffers();
    void destroyCommandBuffers();

    bool createSyncObjects();
    void destroySyncObjects();

private:
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
    VulkanCommandPool* m_commandPool{};
    VulkanDescriptorPool* m_descriptorPool{};

    std::vector<VulkanCommandBuffer*> m_commandBuffers;
};

NS_END
