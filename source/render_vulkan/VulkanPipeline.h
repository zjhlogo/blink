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
#include <spirv-cross/spirv_glsl.hpp>
#include <vulkan/vulkan.h>

#include <vector>

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanSwapchain;
    class VulkanDescriptorPool;
    class VulkanTexture;

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

        static const uint32 InputLocation_Position = 1 << 0;   // inPosition
        static const uint32 InputLocation_Normal = 1 << 1;     // inNormal
        static const uint32 InputLocation_Uv0 = 1 << 2;        // inTexCoord1
        static const uint32 InputLocation_Uv1 = 1 << 3;        // inTexCoord2
        static const uint32 InputLocation_Uv2 = 1 << 4;        // inTexCoord3
        static const uint32 InputLocation_Uv3 = 1 << 5;        // inTexCoord4
        static const uint32 InputLocation_Uv4 = 1 << 6;        // inTexCoord5
        static const uint32 InputLocation_Uv5 = 1 << 7;        // inTexCoord6
        static const uint32 InputLocation_Uv6 = 1 << 8;        // inTexCoord7
        static const uint32 InputLocation_Uv7 = 1 << 9;        // inTexCoord8
        static const uint32 MaxInputLocationMaskBit = 10;

    public:
        VulkanPipeline(VulkanLogicalDevice& logicalDevice, VulkanSwapchain& swapchain, VulkanDescriptorPool& descriptorPool);
        ~VulkanPipeline();

        operator VkPipeline() const { return m_pipeline; };

        bool create(const tstring& vertexShader, const tstring& fragmentShader, int numTextures, bool wireframe = false);
        void destroy();

        VkDescriptorSet updateDescriptorSet(const VkDescriptorBufferInfo& pfuBufferInfo,
                                            const VkDescriptorBufferInfo& pmuBufferInfo,
                                            const VkDescriptorBufferInfo& piuBufferInfo,
                                            const std::vector<VkDescriptorImageInfo>& imageInfos);

        VkDescriptorSetLayout getDestriptorSetLayout() const { return m_descriptorSetLayout; };
        VkPipelineLayout getPipelineLayout() const { return m_pipelineLayout; };

        uint32 getVertexInputMask() const { return m_vertexInputMasks; };

    private:
        VkDescriptorSetLayout createDescriptorSetLayout(int numTextures);
        void destroyDescriptorSetLayout();

        VkPipeline createGraphicsPipeline(const tstring& vertexShader, const tstring& fragmentShader, bool wireframe);
        void destroyGraphicsPipeline();

        VkShaderModule createShaderModule(const tstring& shaderFile);
        uint32 generateVertexInputDesc(std::vector<VkVertexInputBindingDescription>& bindingDesc,
                                     std::vector<VkVertexInputAttributeDescription>& attributeDesc,
                                     const spirv_cross::CompilerGLSL& glsl);
        VkFormat getVertexInputFormat(const spirv_cross::SPIRType& type);

    private:
        VulkanLogicalDevice& m_logicalDevice;
        VulkanSwapchain& m_swapchain;
        VulkanDescriptorPool& m_descriptorPool;

        VkDescriptorSetLayout m_descriptorSetLayout{};
        VkPipelineLayout m_pipelineLayout{};
        VkPipeline m_pipeline{};

        int m_numTextures{};
        std::vector<VkDescriptorSetLayoutBinding> m_layoutBindings;
        std::vector<VkWriteDescriptorSet> m_writeSets;

        uint32 m_vertexInputMasks{};
    };

} // namespace blink
