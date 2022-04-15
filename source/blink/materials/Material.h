/**

    @file      Material.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

#include "../resources/IResource.h"
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
    class IGeometry;

    class Material : public IResource
    {
        friend ResourceMgr;

    public:
        virtual void release() override;

        void bindPipeline(VulkanCommandBuffer& commandBuffer);
        bool bindPerMaterialUniforms(VulkanCommandBuffer& commandBuffer, VulkanUniformBuffer& pmub, VkDescriptorBufferInfo& pmubi);
        bool updateBufferInfos(VulkanCommandBuffer& commandBuffer,
                               IGeometry* geometry,
                               const VkDescriptorBufferInfo& pfubi,
                               const VkDescriptorBufferInfo& pmubi,
                               const VkDescriptorBufferInfo& piubi);

        float getRoughness() const { return m_pmu.roughness; };
        void setRoughness(float roughness) { m_pmu.roughness = roughness; };

        float getMetallic() const { return m_pmu.metallic; };
        void setMetallic(float metallic) { m_pmu.metallic = metallic; };

        const glm::vec3& getColor() const { return m_pmu.color; };
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
        VkPolygonMode m_polygonMode{ VK_POLYGON_MODE_FILL };
        VkPrimitiveTopology m_topology{ VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST };
        std::vector<tstring> m_texturePaths;

        VulkanPipeline* m_pipeline{};
        std::vector<Texture2d*> m_textures;
        std::vector<VkDescriptorImageInfo> m_imageInfos;

        PerMaterialUniforms m_pmu{0.5f, 0.5f, glm::vec3(1.0f, 0.0f, 1.0f)};

    };

} // namespace blink
