/**

    @file      IGeometry.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      2.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <foundation/BaseTypesGlm.h>
#include <vulkan/vulkan.h>

#include <vector>

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanCommandPool;
    class VulkanCommandBuffer;
    class VulkanBuffer;

    class IGeometry
    {
    public:
        IGeometry(VulkanLogicalDevice& logicalDevice, VulkanCommandPool& commandPool);
        virtual ~IGeometry();

        void bindBuffer(VulkanCommandBuffer& commandBuffer);

        uint32 getNumIndices() const { return m_numIndices; }

    protected:
        void destroy();

        bool uploadData(const std::vector<uint16>& indices,
                        const std::vector<glm::vec3>& positions,
                        const std::vector<glm::vec3>& normals,
                        const std::vector<glm::vec2>& uv0s);

    protected:
        VulkanLogicalDevice& m_logicalDevice;
        VulkanCommandPool& m_commandPool;

        VulkanBuffer* m_buffer{};

        VkDeviceSize m_offsetPositions{};
        VkDeviceSize m_sizePositions{};

        VkDeviceSize m_offsetNormals{};
        VkDeviceSize m_sizeNormals{};

        VkDeviceSize m_offsetUv0s{};
        VkDeviceSize m_sizeUv0s{};

        VkDeviceSize m_offsetIndices{};
        VkDeviceSize m_sizeIndices{};
        uint32 m_numIndices{};
    };
} // namespace blink
