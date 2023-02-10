/*********************************************************************
 * \file   VulkanGeometryTriangleList.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/15/2022
 *********************************************************************/
#include "VulkanGeometryTriangleList.h"

#include <render_vulkan/VulkanBuffer.h>
#include <render_vulkan/VulkanPipeline.h>

namespace blink
{
    VulkanGeometryTriangleList::VulkanGeometryTriangleList(VulkanLogicalDevice& logicalDevice)
        : m_logicalDevice(logicalDevice)
    {
        //
    }

    VulkanGeometryTriangleList::~VulkanGeometryTriangleList()
    {
        //
        destroy();
    }

    bool VulkanGeometryTriangleList::uploadData(const std::vector<uint16_t>& indices,
                                                const std::vector<glm::vec3>& positions,
                                                const std::vector<Color>& colors)
    {
        assert(false && "not support");
        return false;
    }

    bool VulkanGeometryTriangleList::uploadData(const std::vector<uint16_t>& indices,
                                                const std::vector<glm::vec3>& positions,
                                                const std::vector<glm::vec3>& normals,
                                                const std::vector<glm::vec2>& uv0s)
    {
        destroy();

        m_offsetIndices = 0;
        m_numVertices = static_cast<uint32_t>(positions.size());
        m_numIndices = static_cast<uint32_t>(indices.size());
        VkDeviceSize sizeIndices = sizeof(uint16_t) * m_numIndices;

        m_offsetPositions = ALIGN_BYTES_4(m_offsetIndices + sizeIndices);
        VkDeviceSize sizePositions = sizeof(glm::vec3) * positions.size();

        m_offsetNormals = ALIGN_BYTES_4(m_offsetPositions + sizePositions);
        VkDeviceSize sizeNormals = sizeof(glm::vec3) * normals.size();

        m_offsetUvs = ALIGN_BYTES_4(m_offsetNormals + sizeNormals);
        VkDeviceSize sizeUv0s = sizeof(glm::vec2) * uv0s.size();

        m_buffer = new VulkanBuffer(m_logicalDevice);
        m_buffer->createBuffer(m_offsetUvs + sizeUv0s,
                               VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
                               | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                               VK_SHARING_MODE_EXCLUSIVE);

        m_buffer->uploadBuffer(
            [&](void* destBuffer, VkDeviceSize destBufferSize)
            {
                memcpy((static_cast<uint8_t*>(destBuffer) + m_offsetIndices), indices.data(), sizeIndices);
                memcpy((static_cast<uint8_t*>(destBuffer) + m_offsetPositions), positions.data(), sizePositions);
                memcpy((static_cast<uint8_t*>(destBuffer) + m_offsetNormals), normals.data(), sizeNormals);
                memcpy((static_cast<uint8_t*>(destBuffer) + m_offsetUvs), uv0s.data(), sizeUv0s);
            });

        m_vertexAttrs = VertexAttrs::Position | VertexAttrs::Normal | VertexAttrs::Uv0;

        m_topology = PrimitiveTopology::TriangleList;

        return true;
    }

    bool VulkanGeometryTriangleList::uploadData(const void* data,
                                                std::size_t dataSize,
                                                uint32_t numVertices,
                                                uint32_t numIndices,
                                                std::size_t offsetPosition,
                                                std::size_t offsetNormal,
                                                std::size_t offsetUv0,
                                                std::size_t offsetIndices)
    {
        destroy();

        m_numVertices = numVertices;
        m_numIndices = numIndices;

        m_offsetPositions = offsetPosition;
        m_offsetNormals = offsetNormal;
        m_offsetUvs = offsetUv0;
        m_offsetIndices = offsetIndices;

        m_buffer = new VulkanBuffer(m_logicalDevice);
        m_buffer->createBuffer(dataSize,
                               VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
                               | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                               VK_SHARING_MODE_EXCLUSIVE);

        m_buffer->uploadBuffer(data, dataSize);

        m_vertexAttrs = VertexAttrs::Position | VertexAttrs::Normal | VertexAttrs::Uv0;

        return true;
    }

    VkDeviceSize VulkanGeometryTriangleList::getVertexInputOffset(VertexAttrs vertexAttrs) const
    {
        if (vertexAttrs == VertexAttrs::Position) { return m_offsetPositions; }
        else if (vertexAttrs == VertexAttrs::Normal) { return m_offsetNormals; }
        else if (vertexAttrs == VertexAttrs::Uv0) { return m_offsetUvs; }

        return 0;
    }

    void VulkanGeometryTriangleList::destroy()
    {
        SAFE_DELETE(m_buffer);

        m_offsetPositions = 0;
        m_offsetNormals = 0;
        m_offsetUvs = 0;
        m_offsetIndices = 0;
    }
} // namespace blink
