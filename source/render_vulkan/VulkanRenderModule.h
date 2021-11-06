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

#include <functional>
#include <vector>

namespace blink
{
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
    class VulkanUniformBuffer;
    class VulkanSemaphore;
    class VulkanFence;

    class VulkanRenderModule
    {
    public:
        typedef std::function<void(VulkanCommandBuffer&, VulkanUniformBuffer&)> RenderCb;

        struct PerFrameUniforms
        {
            alignas(16) glm::mat4 matWorldToCamera;
            alignas(16) glm::mat4 matCameraToProjection;
            alignas(16) glm::mat4 matWorldToProjection;
            alignas(16) glm::mat3x4 matWorldToCameraInvT;
            alignas(16) glm::vec3 cameraPos;
            alignas(16) glm::vec3 cameraDir;
        };

    public:
        VulkanRenderModule();
        virtual ~VulkanRenderModule();

        bool createDevice(const glm::ivec2& deviceSize);
        void destroyDevice();

        bool processEvent();
        void render(const RenderCb& cb);

        void waitIdle();

        VulkanLogicalDevice& getLogicalDevice() const { return *m_logicalDevice; };
        VulkanSwapchain& getSwapchain() const { return *m_swapchain; };
        VulkanCommandPool& getCommandPool() const { return *m_commandPool; };
        VulkanDescriptorPool& getDescriptorPool() const { return *m_descriptorPool; };

    private:
        bool createSyncObjects();
        void destroySyncObjects();

    private:
        VulkanFence* m_acquireImageFence{};

        VulkanWindow* m_window{};
        VulkanContext* m_context{};
        VulkanLogicalDevice* m_logicalDevice{};
        VulkanSwapchain* m_swapchain{};
        VulkanCommandPool* m_commandPool{};
        VulkanDescriptorPool* m_descriptorPool{};
        VulkanCommandBuffer* m_commandBuffer{};
        VulkanUniformBuffer* m_uniformBuffer{};
        PerFrameUniforms m_perFrameUniforms{};
    };

} // namespace blink
