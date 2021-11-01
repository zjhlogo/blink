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

    class VulkanPipeline
    {
    public:
        VulkanPipeline(VulkanLogicalDevice& logicalDevice, VulkanSwapchain& swapchain);
        ~VulkanPipeline();

        operator VkPipeline() const { return m_pipeline; };

        bool create();
        void destroy();

        bool recreatePipeline();

        VkDescriptorSetLayout getDestriptorSetLayout() const { return m_descriptorSetLayout; };
        VkPipelineLayout getPipelineLayout() const { return m_pipelineLayout; };

    private:
        VkDescriptorSetLayout createDescriptorSetLayout();
        void destroyDescriptorSetLayout();

        VkPipeline createGraphicsPipeline(uint32_t width, uint32_t height);
        void destroyGraphicsPipeline();

        VkShaderModule createShaderModule(const tstring& shaderFile);

    private:
        VulkanLogicalDevice& m_logicalDevice;
        VulkanSwapchain& m_swapchain;

        VkDescriptorSetLayout m_descriptorSetLayout{};
        VkPipelineLayout m_pipelineLayout{};
        VkPipeline m_pipeline{};
    };

} // namespace blink
