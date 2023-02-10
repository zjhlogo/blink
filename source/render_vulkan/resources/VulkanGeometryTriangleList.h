/*********************************************************************
 * \file   VulkanGeometryTriangleList.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/15/2022
 *********************************************************************/
#pragma once

#include "VulkanGeometry.h"

namespace blink
{
    class VulkanLogicalDevice;

    class VulkanGeometryTriangleList final : public VulkanGeometry
    {
    public:
        explicit VulkanGeometryTriangleList(VulkanLogicalDevice& logicalDevice);
        ~VulkanGeometryTriangleList() override;

        VulkanGeometryTriangleList(const VulkanGeometryTriangleList& triangleList) = delete;
        VulkanGeometryTriangleList(VulkanGeometryTriangleList&& triangleList) = delete;
        VulkanGeometryTriangleList& operator=(const VulkanGeometryTriangleList& triangleList) = delete;
        VulkanGeometryTriangleList& operator=(VulkanGeometryTriangleList&& triangleList) = delete;

        bool uploadData(const std::vector<uint16_t>& indices, const std::vector<glm::vec3>& positions, const std::vector<Color>& colors) override;

        bool uploadData(const std::vector<uint16_t>& indices,
                        const std::vector<glm::vec3>& positions,
                        const std::vector<glm::vec3>& normals,
                        const std::vector<glm::vec2>& uv0s) override;

        bool uploadData(const void* data,
                        std::size_t dataSize,
                        uint32_t numVertices,
                        uint32_t numIndices,
                        std::size_t offsetPosition,
                        std::size_t offsetNormal,
                        std::size_t offsetUv0,
                        std::size_t offsetIndices) override;

        VulkanBuffer* getVulkanBuffer() const override { return m_buffer; }
        VkDeviceSize getVertexInputOffset(VertexAttrs vertexAttrs) const override;
        VkDeviceSize getIndicesOffset() const override { return m_offsetIndices; }

    protected:
        void destroy();

    protected:
        VulkanLogicalDevice& m_logicalDevice;
        VulkanBuffer* m_buffer{};

        VkDeviceSize m_offsetPositions{};
        VkDeviceSize m_offsetNormals{};
        VkDeviceSize m_offsetUvs{};
        VkDeviceSize m_offsetIndices{};
    };
} // namespace blink
