/*********************************************************************
 * \file   VulkanMaterial.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#pragma once

#include "../VulkanPipeline.h"
#include "VulkanGeometry.h"

#include <core/resources/IMaterial.h>

#include <vector>

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanRenderPass;
    class VulkanCommandBuffer;
    class VulkanUniformBuffer;

    class VulkanMaterial final : public IMaterial
    {
    private:
        struct TextureInfo
        {
            tstring name;
            tstring path;
            VulkanTexture* texture;
        };

    public:
        VulkanMaterial(VulkanLogicalDevice& logicalDevice, VulkanRenderPass& renderPass);
        ~VulkanMaterial() override;

        VulkanMaterial(const VulkanMaterial&) = delete;
        VulkanMaterial(VulkanMaterial&&) = delete;
        VulkanMaterial& operator=(const VulkanMaterial&) = delete;
        VulkanMaterial& operator=(VulkanMaterial&&) = delete;

        bool create(const tstring& filePath);
        void destroy();

        void bindPipeline(const VulkanCommandBuffer& commandBuffer);
        bool uploadUniformDescriptorBufferInfo(UniformBinding binding, const VkDescriptorBufferInfo& bufferInfo);
        bool updateBufferInfos(const VulkanCommandBuffer& commandBuffer, const VulkanGeometry* geometry);

        [[nodiscard]] VulkanPipeline& getPipeline() const { return *m_pipeline; }

    protected:
        bool setUniform(const tstring& memberName, UniformType type, const void* data) override;
        bool getUniform(void* dataOut, const tstring& memberName, UniformType type) override;

    private:
        bool loadTextures();

    private:
        VulkanLogicalDevice& m_logicalDevice;
        VulkanRenderPass& m_renderPass;

        tstring m_filePath;
        tstring m_vertexShader;
        tstring m_fragmentShader;
        VkPolygonMode m_polygonMode{VK_POLYGON_MODE_FILL};
        PrimitiveTopology m_topology{PrimitiveTopology::TriangleList};

        bool m_zTestEnable{true};
        bool m_zWriteEnable{true};

        std::vector<VulkanPipeline::DescriptorInfo> m_descriptorInfoList;
        std::unordered_map<tstring, TextureInfo> m_textureInfoMap;

        VulkanPipeline* m_pipeline{};
    };
} // namespace blink
