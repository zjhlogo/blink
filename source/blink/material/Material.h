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
#include "../type/RenderTypes.h"

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
    class Texture2d;
    class ResourceMgr;
    class Geometry;

    class Material : public IResource
    {
        friend ResourceMgr;

    public:
        void bindPipeline(VulkanCommandBuffer& commandBuffer);
        bool bindPerMaterialUniforms(VulkanCommandBuffer& commandBuffer, VulkanUniformBuffer& pmub, VkDescriptorBufferInfo& pmubi);
        bool updateBufferInfos(VulkanCommandBuffer& commandBuffer,
                               Geometry* geometry,
                               const VkDescriptorBufferInfo& pfubi,
                               const VkDescriptorBufferInfo& pmubi,
                               const VkDescriptorBufferInfo& piubi);

        VulkanPipeline& getPipeline() { return *m_pipeline; };

    private:
        Material(VulkanLogicalDevice& logicalDevice, VulkanSwapchain& swapchain, VulkanDescriptorPool& descriptorPool);
        ~Material();

        bool create(const tstring& filePath);
        void destroy();

        bool loadConfigFromFile(const tstring& filePath);
        bool loadTextures();

    private:
        VulkanLogicalDevice& m_logicalDevice;
        VulkanSwapchain& m_swapchain;
        VulkanDescriptorPool& m_descriptorPool;

        tstring m_filePath;
        tstring m_vertexShader;
        tstring m_fragmentShader;
        bool m_wireframe{};
        std::vector<tstring> m_texturePaths;

        VulkanPipeline* m_pipeline{};
        std::vector<Texture2d*> m_textures;
        std::vector<VkDescriptorImageInfo> m_imageInfos;

        PerMaterialUniforms m_pmu{0.5f, 0.5f, glm::vec3(1.0f, 0.0f, 1.0f)};
    };

} // namespace blink
