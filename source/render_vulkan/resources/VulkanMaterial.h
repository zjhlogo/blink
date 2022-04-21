
/*********************************************************************
 * \file   VulkanMaterial.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#pragma once

#include "VulkanGeometry.h"

#include <core/resources/IMaterial.h>
#include <core/resources/ITexture2d.h>
#include <vulkan/vulkan.h>

#include <vector>

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanSwapchain;
    class VulkanPipeline;
    class VulkanCommandBuffer;
    class VulkanUniformBuffer;

    class VulkanMaterial : public IMaterial
    {
    public:
        VulkanMaterial(VulkanLogicalDevice& logicalDevice, VulkanSwapchain& swapchain);
        ~VulkanMaterial();

        bool create(const tstring& filePath);
        void destroy();

        bool loadConfigFromFile(const tstring& filePath);
        bool loadTextures();

        virtual bool recreate() override;

        void bindPipeline(VulkanCommandBuffer& commandBuffer);

        bool bindPerMaterialUniforms(VulkanCommandBuffer& commandBuffer,
                                     VulkanUniformBuffer& pmub,
                                     VkDescriptorBufferInfo& pmubi);

        bool updateBufferInfos(VulkanCommandBuffer& commandBuffer,
                               VulkanGeometry* geometry,
                               const VkDescriptorBufferInfo& pfubi,
                               const VkDescriptorBufferInfo& pmubi,
                               const VkDescriptorBufferInfo& piubi);

        virtual float getRoughness() const override { return m_pmu.roughness; };
        virtual void setRoughness(float roughness) override { m_pmu.roughness = roughness; };

        virtual float getMetallic() const override { return m_pmu.metallic; };
        virtual void setMetallic(float metallic) override { m_pmu.metallic = metallic; };

        virtual const glm::vec3& getColor() const override { return m_pmu.color; };
        virtual void setColor(const glm::vec3& color) override { m_pmu.color = color; };

        VulkanPipeline& getPipeline() const { return *m_pipeline; };

    private:
        VulkanLogicalDevice& m_logicalDevice;
        VulkanSwapchain& m_swapchain;

        tstring m_filePath;
        tstring m_vertexShader;
        tstring m_fragmentShader;
        VkPolygonMode m_polygonMode{VK_POLYGON_MODE_FILL};
        PrimitiveTopology m_topology{PrimitiveTopology::TriangleList};
        std::vector<tstring> m_texturePaths;

        VulkanPipeline* m_pipeline{};
        std::vector<ITexture2d*> m_textures;
        std::vector<VkDescriptorImageInfo> m_imageInfos;

        PerMaterialUniforms m_pmu{0.5f, 0.5f, glm::vec3(1.0f, 0.0f, 1.0f)};
    };

} // namespace blink
