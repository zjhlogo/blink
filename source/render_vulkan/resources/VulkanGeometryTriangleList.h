
/*********************************************************************
 * \file   VulkanGeometryTriangleList.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/15/2022
 *********************************************************************/
#pragma once

#include <core/resources/IGeometryTriangleList.h>
#include <vulkan/vulkan_core.h>

#include <vector>

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanCommandBuffer;
    class VulkanBuffer;
    class ResourceMgr;

    class VulkanGeometryTriangleList : public IGeometryTriangleList
    {
        friend ResourceMgr;

    public:
        virtual bool uploadData(const std::vector<uint16>& indices,
                                const std::vector<glm::vec3>& positions,
                                const std::vector<glm::vec3>& normals,
                                const std::vector<glm::vec2>& uv0s) override;

        virtual bool uploadData(const void* data,
                                std::size_t dataSize,
                                uint32 numVertices,
                                uint32 numIndices,
                                std::size_t offsetPosition,
                                std::size_t offsetNormal,
                                std::size_t offsetUv0,
                                std::size_t offsetIndices) override;

        VulkanBuffer* getVulkanBuffer() const { return m_buffer; }
        VkDeviceSize getVertexInputOffset(uint32 inputMask) const;
        VkDeviceSize getIndicesOffset() const { return m_offsetIndices; }

    protected:
        VulkanGeometryTriangleList(VulkanLogicalDevice& logicalDevice);
        virtual ~VulkanGeometryTriangleList();

        void destroy();

    protected:
        VulkanLogicalDevice& m_logicalDevice;
        VulkanBuffer* m_buffer{};

        VkDeviceSize m_offsetPositions{};
        VkDeviceSize m_offsetNormals{};
        VkDeviceSize m_offsetUv0s{};
        VkDeviceSize m_offsetIndices{};
    };
} // namespace blink
