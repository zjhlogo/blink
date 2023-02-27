/*!
 * \file VulkanPipeline.h
 *
 * \author zjhlogo
 * \date 2020/02/26
 *
 *
 */
#pragma once
#include "VulkanBase.h"
#include "VulkanUniformBlock.h"

#include <core/resources/IGeometry.h>
#include <core/types/VertexAttrs.h>

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanRenderPass;
    class VulkanTexture;
    class VulkanCommandBuffer;
    class ITexture2d;

    class VulkanPipeline
    {
    public:
        union DescriptorInfo
        {
            VkDescriptorImageInfo imageInfo;
            VkDescriptorBufferInfo bufferInfo;
        };

        struct PushConstantData
        {
            glm::mat4 localToWorld;
            glm::mat3x4 localToWorldInvT;
        };

    public:
        VulkanPipeline(VulkanLogicalDevice& logicalDevice, VulkanRenderPass& renderPass);
        ~VulkanPipeline();

        VulkanPipeline(const VulkanPipeline&) = delete;
        VulkanPipeline(VulkanPipeline&&) = delete;
        VulkanPipeline& operator=(const VulkanPipeline&) = delete;
        VulkanPipeline& operator=(VulkanPipeline&&) = delete;

        explicit operator VkPipeline() const { return m_pipeline; }

        bool create(const tstring& vertexShader, const tstring& fragmentShader, VkPolygonMode polygonMode, PrimitiveTopology topology);
        void destroy();

        bool bindDescriptorSets(const VulkanCommandBuffer& commandBuffer, const std::vector<VulkanPipeline::DescriptorInfo>& descriptorInfoList);

        [[nodiscard]] VkDescriptorSetLayout getDescriptorSetLayout() const { return m_descriptorSetLayout; }
        [[nodiscard]] VkPipelineLayout getPipelineLayout() const { return m_pipelineLayout; }
        [[nodiscard]] VertexAttrs getVertexAttrFlags() const { return m_vertexAttrs; }

        [[nodiscard]] std::size_t getWriteSetCount() const { return m_writeSets.size(); }
        VulkanUniformBlock* getUniformBlock(UniformBinding binding);
        int getUniformWriteSetIndexFromBinding(UniformBinding binding);
        [[nodiscard]] const std::unordered_map<uint32_t, int>& getUniformWriteSetIndexMap() const { return m_uniformWriteSetIndexMap; }
        [[nodiscard]] const std::unordered_map<tstring, int>& getTextureWriteSetIndexMap() const { return m_textureWriteSetIndexMap; }

    private:
        VkDescriptorSetLayout createDescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding>& layoutBindings);
        void destroyDescriptorSetLayout();

        VkPipeline createGraphicsPipeline(const std::vector<uint8_t>& vertexShaderCode,
                                          const std::vector<uint8_t>& fragmentShaderCode,
                                          const std::vector<VkVertexInputBindingDescription>& bindings,
                                          const std::vector<VkVertexInputAttributeDescription>& attributes,
                                          VkPolygonMode polygonMode,
                                          VkPrimitiveTopology topology,
                                          const glm::ivec2& surfaceSize);
        void destroyGraphicsPipeline();

        VkShaderModule createShaderModule(const std::vector<uint8_t>& shaderCode);

        static VertexAttrs generateVertexInputDesc(std::vector<VkVertexInputBindingDescription>& bindingDesc,
                                                   std::vector<VkVertexInputAttributeDescription>& attributeDesc,
                                                   const std::vector<uint8_t>& vertexShaderCode);

        void generateDescriptorSetLayout(std::vector<VkDescriptorSetLayoutBinding>& layoutBindings,
                                         const std::vector<uint8_t>& shaderCode,
                                         uint32_t shaderStageBits);

    private:
        VulkanLogicalDevice& m_logicalDevice;
        VulkanRenderPass& m_renderPass;

        VkDescriptorSetLayout m_descriptorSetLayout{};
        VkPipelineLayout m_pipelineLayout{};
        VkPipeline m_pipeline{};

        std::vector<VkWriteDescriptorSet> m_writeSets;
        std::unordered_map<uint32_t, int> m_uniformWriteSetIndexMap;
        std::unordered_map<tstring, int> m_textureWriteSetIndexMap;

        std::unordered_map<uint32_t, VulkanUniformBlock> m_uniformBlocks;

        VertexAttrs m_vertexAttrs{VertexAttrs::None};
        tstring m_vertexShader;
        tstring m_fragmentShader;
        VkPolygonMode m_polygonMode{VK_POLYGON_MODE_FILL};
        VkPrimitiveTopology m_topology{VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST};
    };
} // namespace blink
