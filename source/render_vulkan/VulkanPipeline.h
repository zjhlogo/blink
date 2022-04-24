/*!
 * \file VulkanPipeline.h
 *
 * \author zjhlogo
 * \date 2020/02/26
 *
 *
 */
#pragma once
#include <core/resources/IGeometry.h>
#include <core/types/VertexAttrs.h>
#include <vulkan/vulkan.h>

#include <unordered_map>

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanSwapchain;
    class VulkanTexture;
    class VulkanCommandBuffer;

    class VulkanPipeline
    {
    public:
        VulkanPipeline(VulkanLogicalDevice& logicalDevice, VulkanSwapchain& swapchain);
        ~VulkanPipeline();

        operator VkPipeline() const { return m_pipeline; };

        bool create(const tstring& vertexShader,
                    const tstring& fragmentShader,
                    VkPolygonMode polygonMode,
                    PrimitiveTopology topology);
        bool recreate();
        void destroy();

        bool bindDescriptorSets(VulkanCommandBuffer& commandBuffer,
                                const VkDescriptorBufferInfo& pfuBufferInfo,
                                const VkDescriptorBufferInfo& pmuBufferInfo,
                                const VkDescriptorBufferInfo& piuBufferInfo,
                                const std::vector<VkDescriptorImageInfo>& imageInfos);

        VkDescriptorSetLayout getDestriptorSetLayout() const { return m_descriptorSetLayout; };
        VkPipelineLayout getPipelineLayout() const { return m_pipelineLayout; };

        VertexAttrs getVertexAttrFlags() const { return m_vertexAttrs; };

    private:
        VkDescriptorSetLayout createDescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding>& layoutBindings);
        void destroyDescriptorSetLayout();

        VkPipeline createGraphicsPipeline(const std::vector<uint8>& vertexShaderCode,
                                          const std::vector<uint8>& fragmentShaderCode,
                                          const std::vector<VkVertexInputBindingDescription>& bindings,
                                          const std::vector<VkVertexInputAttributeDescription>& attributes,
                                          VkPolygonMode polygonMode,
                                          VkPrimitiveTopology topology);
        void destroyGraphicsPipeline();

        VkShaderModule createShaderModule(const std::vector<uint8>& shaderCode);
        VertexAttrs generateVertexInputDesc(std::vector<VkVertexInputBindingDescription>& bindingDesc,
                                            std::vector<VkVertexInputAttributeDescription>& attributeDesc,
                                            const std::vector<uint8>& vertexShaderCode);
        size_t generateDescriptorSetLayout(std::vector<VkDescriptorSetLayoutBinding>& layoutBindings,
                                           std::vector<VkWriteDescriptorSet>& writeSets,
                                           std::unordered_map<tstring, int>& writeSetNameIndexMap,
                                           const std::vector<uint8>& fragmentShaderCode);

    private:
        VulkanLogicalDevice& m_logicalDevice;
        VulkanSwapchain& m_swapchain;

        VkDescriptorSetLayout m_descriptorSetLayout{};
        VkPipelineLayout m_pipelineLayout{};
        VkPipeline m_pipeline{};

        int m_numTextures{};
        std::vector<VkWriteDescriptorSet> m_writeSets;
        std::unordered_map<tstring, int> m_writeSetNameIndexMap;
        VertexAttrs m_vertexAttrs{VertexAttrs::None};
        tstring m_vertexShader;
        tstring m_fragmentShader;
        VkPolygonMode m_polygonMode{VK_POLYGON_MODE_FILL};
        VkPrimitiveTopology m_topology{VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST};
    };

} // namespace blink
