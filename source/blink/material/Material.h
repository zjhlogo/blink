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
    class Texture2d;
    class ResourceMgr;
    class Geometry;

    class Material : public IResource
    {
        friend ResourceMgr;

    public:
        virtual void release() override;

        void bindPipeline(VulkanCommandBuffer& commandBuffer);
        bool bindPerMaterialUniforms(VulkanCommandBuffer& commandBuffer, VulkanUniformBuffer& pmub, VkDescriptorBufferInfo& pmubi);
        bool updateBufferInfos(VulkanCommandBuffer& commandBuffer,
                               Geometry* geometry,
                               const VkDescriptorBufferInfo& pfubi,
                               const VkDescriptorBufferInfo& pmubi,
                               const VkDescriptorBufferInfo& piubi);

        void setRoughness(float roughness) { m_pmu.roughness = roughness; };
        void setMetallic(float metallic) { m_pmu.metallic = metallic; };
        void setColor(const glm::vec3& color) { m_pmu.color = color; };

        VulkanPipeline& getPipeline() const { return *m_pipeline; };

    private:
        Material(VulkanLogicalDevice& logicalDevice, VulkanSwapchain& swapchain);
        ~Material();

        bool create(const tstring& filePath);
        bool recreate();
        void destroy();

        bool loadConfigFromFile(const tstring& filePath);
        bool loadTextures();

    private:
        VulkanLogicalDevice& m_logicalDevice;
        VulkanSwapchain& m_swapchain;

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
