
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
#include <core/resources/ITexture2d.h>

#include <vector>

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanSwapchain;
    class VulkanCommandBuffer;
    class VulkanUniformBuffer;

    class VulkanMaterial : public IMaterial
    {
    private:
        struct TextureInfo
        {
            tstring name;
            tstring path;
            VulkanTexture* texture;
        };

    public:
        VulkanMaterial(VulkanLogicalDevice& logicalDevice, VulkanSwapchain& swapchain);
        ~VulkanMaterial();

        bool create(const tstring& filePath);
        void destroy();

        virtual bool recreate() override;

        void bindPipeline(VulkanCommandBuffer& commandBuffer);
        bool uploadUniformDescriptorBufferInfo(UniformBinding binding, const VkDescriptorBufferInfo& bufferInfo);
        bool updateBufferInfos(VulkanCommandBuffer& commandBuffer, VulkanGeometry* geometry);

        VulkanPipeline& getPipeline() const { return *m_pipeline; };

    protected:
        virtual bool setUniform(const tstring& memberName, UniformType type, const void* data) override;
        virtual bool getUniform(void* dataOut, const tstring& memberName, UniformType type) override;

    private:
        bool loadTextures();

    private:
        VulkanLogicalDevice& m_logicalDevice;
        VulkanSwapchain& m_swapchain;

        tstring m_filePath;
        tstring m_vertexShader;
        tstring m_fragmentShader;
        VkPolygonMode m_polygonMode{VK_POLYGON_MODE_FILL};
        PrimitiveTopology m_topology{PrimitiveTopology::TriangleList};

        std::vector<VulkanPipeline::DescriptorInfo> m_descriptorInfoList;
        std::unordered_map<tstring, TextureInfo> m_textureInfoMap;

        VulkanPipeline* m_pipeline{};
    };

} // namespace blink
