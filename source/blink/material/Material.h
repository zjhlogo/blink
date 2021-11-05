/**

    @file      Material.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

#include "../resource/IResource.h"

#include <foundation/BaseTypesGlm.h>
#include <vulkan/vulkan.h>

#include <vector>

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanSwapchain;
    class VulkanPipeline;
    class VulkanCommandBuffer;
    class VulkanUniformBuffer;
    class VulkanDescriptorPool;
    class VulkanCommandPool;
    class Texture2d;
    class ResourceMgr;

    class Material : public IResource
    {
        friend ResourceMgr;

    public:
        void bindPipeline(VulkanCommandBuffer& commandBuffer);
        bool bindUniformBuffer(VulkanCommandBuffer& commandBuffer, VulkanUniformBuffer& uniformBuffer, const glm::vec3& pos, const glm::quat& rot);

        VulkanPipeline& getPipeline() { return *m_pipeline; };

    private:
        Material(VulkanLogicalDevice& logicalDevice, VulkanSwapchain& swapchain, VulkanDescriptorPool& descriptorPool, VulkanCommandPool& commandPool);
        ~Material();

        bool create(const tstring& filePath);
        void destroy();

        bool loadConfigFromFile(const tstring& filePath);
        bool loadTextures();

    private:
        VulkanLogicalDevice& m_logicalDevice;
        VulkanSwapchain& m_swapchain;
        VulkanDescriptorPool& m_descriptorPool;
        VulkanCommandPool& m_commandPool;

        tstring m_filePath;
        tstring m_vertexShader;
        tstring m_fragmentShader;
        bool m_wireframe{false};
        std::vector<tstring> m_texturePaths;

        VulkanPipeline* m_pipeline{};
        std::vector<Texture2d*> m_textures;
        std::vector<VkDescriptorImageInfo> m_imageInfos;
    };

} // namespace blink
