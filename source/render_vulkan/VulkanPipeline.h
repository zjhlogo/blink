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

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanSwapchain;
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
                                const std::vector<VulkanPipeline::DescriptorInfo>& descriptorInfoList);

        VkDescriptorSetLayout getDestriptorSetLayout() const { return m_descriptorSetLayout; };
        VkPipelineLayout getPipelineLayout() const { return m_pipelineLayout; };
        VertexAttrs getVertexAttrFlags() const { return m_vertexAttrs; };

        const std::vector<VkWriteDescriptorSet>& getWriteDescriptorSets() const { return m_writeSets; };
        const std::vector<tstring>& getWriteDescriptorSetNames() const { return m_writeSetNames; };
        int getPerCameraUniformIndex() const { return m_perCameraUniformIndex; };
        int getPerInstanceUniformIndex() const { return m_perInstanceUniformIndex; };

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
        void generateDescriptorSetLayout(std::vector<VkDescriptorSetLayoutBinding>& layoutBindings,
                                         std::vector<VkWriteDescriptorSet>& writeSets,
                                         std::vector<tstring>& writeSetNames,
                                         int& perCameraUniformIndex,
                                         int& perInstanceUniformIndex,
                                         const std::vector<uint8>& fragmentShaderCode);

    private:
        VulkanLogicalDevice& m_logicalDevice;
        VulkanSwapchain& m_swapchain;

        VkDescriptorSetLayout m_descriptorSetLayout{};
        VkPipelineLayout m_pipelineLayout{};
        VkPipeline m_pipeline{};

        std::vector<VkWriteDescriptorSet> m_writeSets;
        std::vector<tstring> m_writeSetNames;
        int m_perCameraUniformIndex{-1};
        int m_perInstanceUniformIndex{-1};

        VertexAttrs m_vertexAttrs{VertexAttrs::None};
        tstring m_vertexShader;
        tstring m_fragmentShader;
        VkPolygonMode m_polygonMode{VK_POLYGON_MODE_FILL};
        VkPrimitiveTopology m_topology{VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST};
    };

} // namespace blink
