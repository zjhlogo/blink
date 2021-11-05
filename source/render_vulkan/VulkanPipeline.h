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

    class VulkanPipeline
    {
    public:
        enum PredefineBindingIndex
        {
            PerFrameUniformIndex = 0,
            PerInstanceUniformIndex,
            SamplerUniformIndexBegin
        };

    public:
        VulkanPipeline(VulkanLogicalDevice& logicalDevice, VulkanSwapchain& swapchain, VulkanDescriptorPool& descriptorPool);
        ~VulkanPipeline();

        operator VkPipeline() const { return m_pipeline; };

        bool create(const std::vector<VkVertexInputBindingDescription>& bindings,
                    const std::vector<VkVertexInputAttributeDescription>& attributes,
                    const tstring& vertexShader,
                    const tstring& fragmentShader,
                    int numTextures,
                    bool wireframe = false);
        void destroy();

        VkDescriptorSet updateDescriptorSet(const VkDescriptorBufferInfo& pfuBufferInfo,
                                            const VkDescriptorBufferInfo& piuBufferInfo,
                                            const std::vector<VulkanTexture*>& textures);

        VkDescriptorSetLayout getDestriptorSetLayout() const { return m_descriptorSetLayout; };
        VkPipelineLayout getPipelineLayout() const { return m_pipelineLayout; };

    private:
        VkDescriptorSetLayout createDescriptorSetLayout(int numTextures);
        void destroyDescriptorSetLayout();

        VkPipeline createGraphicsPipeline(const std::vector<VkVertexInputBindingDescription>& bindings,
                                          const std::vector<VkVertexInputAttributeDescription>& attributes,
                                          const tstring& vertexShader,
                                          const tstring& fragmentShader,
                                          bool wireframe);
        void destroyGraphicsPipeline();

        VkShaderModule createShaderModule(const tstring& shaderFile);

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
    };

} // namespace blink
