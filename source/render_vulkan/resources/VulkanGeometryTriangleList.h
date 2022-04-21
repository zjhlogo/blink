
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

    class VulkanGeometryTriangleList : public VulkanGeometry
    {
    public:
        VulkanGeometryTriangleList(VulkanLogicalDevice& logicalDevice);
        virtual ~VulkanGeometryTriangleList();

        virtual bool uploadData(const std::vector<uint16>& indices,
                                const std::vector<glm::vec3>& positions,
                                const std::vector<Color>& colors) override;

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

        virtual VulkanBuffer* getVulkanBuffer() const override { return m_buffer; }
        virtual VkDeviceSize getVertexInputOffset(uint32 inputMask) const override;
        virtual VkDeviceSize getIndicesOffset() const override { return m_offsetIndices; }

    protected:
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
