/**

    @file      Material.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <foundation/BaseTypesGlm.h>

#include <vector>

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanSwapchain;
    class VulkanPipeline;
    class VulkanCommandBuffer;
    class VulkanUniformBuffer;
    class VulkanDescriptorPool;
    class VulkanTexture;

    class Material
    {
    public:
        Material(VulkanLogicalDevice& logicalDevice, VulkanSwapchain& swapchain, VulkanDescriptorPool& descriptorPool);
        ~Material();

        bool create();
        void destroy();

        void setTexture(VulkanTexture* texture);

        void bindPipeline(VulkanCommandBuffer& commandBuffer);
        bool bindUniformBuffer(VulkanCommandBuffer& commandBuffer,
                               VulkanUniformBuffer& uniformBuffer,
                               VulkanDescriptorPool& descriptorPool,
                               const glm::vec3& pos,
                               const glm::quat& rot);

        VulkanPipeline& getPipeline() { return *m_pipeline; };

    private:
        VulkanLogicalDevice& m_logicalDevice;
        VulkanSwapchain& m_swapchain;
        VulkanDescriptorPool& m_descriptorPool;

        VulkanPipeline* m_pipeline{};

        // the datas not belong to material
        VulkanTexture* m_texture{};
    };

} // namespace blink
