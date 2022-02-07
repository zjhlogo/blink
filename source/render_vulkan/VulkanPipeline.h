/*!
 * \file VulkanPipeline.h
 *
 * \author zjhlogo
 * \date 2020/02/26
 *
 *
 */
#pragma once
#include <foundation/BaseTypes.h>
#include <vulkan/vulkan.h>

#include <vector>

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanSwapchain;
    class VulkanDescriptorPool;
    class VulkanTexture;
    class VulkanCommandBuffer;

    class VulkanPipeline
    {
    public:
        enum PredefineBindingIndex
        {
            PerFrameUniformIndex = 0,
            PerMaterialUniformIndex,
            PerInstanceUniformIndex,
            SamplerUniformIndexBegin
        };

        static const uint32 InputLocation_Position = 1 << 0; // inPosition
        static const uint32 InputLocation_Normal = 1 << 1;   // inNormal
        static const uint32 InputLocation_Uv0 = 1 << 2;      // inTexCoord1
        static const uint32 InputLocation_Uv1 = 1 << 3;      // inTexCoord2
        static const uint32 InputLocation_Uv2 = 1 << 4;      // inTexCoord3
        static const uint32 InputLocation_Uv3 = 1 << 5;      // inTexCoord4
        static const uint32 InputLocation_Uv4 = 1 << 6;      // inTexCoord5
        static const uint32 InputLocation_Uv5 = 1 << 7;      // inTexCoord6
        static const uint32 InputLocation_Uv6 = 1 << 8;      // inTexCoord7
        static const uint32 InputLocation_Uv7 = 1 << 9;      // inTexCoord8
        static const uint32 MaxInputLocationMaskBit = 10;

    public:
        VulkanPipeline(VulkanLogicalDevice& logicalDevice, VulkanSwapchain& swapchain, VulkanDescriptorPool& descriptorPool);
        ~VulkanPipeline();

        operator VkPipeline() const { return m_pipeline; };

        bool create(const tstring& vertexShader, const tstring& fragmentShader, bool wireframe = false);
        bool recreate();
        void destroy();

        bool bindDescriptorSets(VulkanCommandBuffer& commandBuffer,
                                const VkDescriptorBufferInfo& pfuBufferInfo,
                                const VkDescriptorBufferInfo& pmuBufferInfo,
                                const VkDescriptorBufferInfo& piuBufferInfo,
                                const std::vector<VkDescriptorImageInfo>& imageInfos);

        VkDescriptorSetLayout getDestriptorSetLayout() const { return m_descriptorSetLayout; };
        VkPipelineLayout getPipelineLayout() const { return m_pipelineLayout; };

        uint32 getVertexInputMask() const { return m_vertexInputMasks; };

    private:
        VkDescriptorSetLayout createDescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding>& layoutBindings);
        void destroyDescriptorSetLayout();

        VkPipeline createGraphicsPipeline(const std::vector<uint8>& vertexShaderCode,
                                          const std::vector<uint8>& fragmentShaderCode,
                                          const std::vector<VkVertexInputBindingDescription>& bindings,
                                          const std::vector<VkVertexInputAttributeDescription>& attributes,
                                          bool wireframe);
        void destroyGraphicsPipeline();

        VkShaderModule createShaderModule(const std::vector<uint8>& shaderCode);
        uint32 generateVertexInputDesc(std::vector<VkVertexInputBindingDescription>& bindingDesc,
                                       std::vector<VkVertexInputAttributeDescription>& attributeDesc,
                                       const std::vector<uint8>& vertexShaderCode);
        size_t generateDescriptorSetLayout(std::vector<VkDescriptorSetLayoutBinding>& layoutBindings,
                                           std::vector<VkWriteDescriptorSet>& writeSets,
                                           const std::vector<uint8>& fragmentShaderCode);

    private:
        VulkanLogicalDevice& m_logicalDevice;
        VulkanSwapchain& m_swapchain;
        VulkanDescriptorPool& m_descriptorPool;

        VkDescriptorSetLayout m_descriptorSetLayout{};
        VkPipelineLayout m_pipelineLayout{};
        VkPipeline m_pipeline{};

        int m_numTextures{};
        std::vector<VkWriteDescriptorSet> m_writeSets;
        uint32 m_vertexInputMasks{};
        tstring m_vertexShader;
        tstring m_fragmentShader;
        bool m_wireframe{};

    };

} // namespace blink
