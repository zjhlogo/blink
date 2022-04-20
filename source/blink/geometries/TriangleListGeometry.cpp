
/*********************************************************************
 * \file   TriangleListGeometry.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/15/2022
 *********************************************************************/
#include "TriangleListGeometry.h"

#include <blink/resources/ResourceMgr.h>
#include <render_vulkan/VulkanBuffer.h>
#include <render_vulkan/VulkanPipeline.h>

namespace blink
{
    TriangleListGeometry::TriangleListGeometry(VulkanLogicalDevice& logicalDevice)
        : IGeometry(logicalDevice)
    {
        //
    }

    TriangleListGeometry::~TriangleListGeometry()
    {
        //
        destroy();
    }

    bool TriangleListGeometry::uploadData(const std::vector<uint16>& indices,
                                          const std::vector<glm::vec3>& positions,
                                          const std::vector<glm::vec3>& normals,
                                          const std::vector<glm::vec2>& uv0s)
    {
        destroy();

        m_offsetIndices = 0;
        m_numIndices = static_cast<uint32>(indices.size());
        VkDeviceSize sizeIndices = sizeof(uint16) * m_numIndices;

        m_offsetPositions = ALIGN_BYTES_4(m_offsetIndices + sizeIndices);
        VkDeviceSize sizePositions = sizeof(glm::vec3) * positions.size();

        m_offsetNormals = ALIGN_BYTES_4(m_offsetPositions + sizePositions);
        VkDeviceSize sizeNormals = sizeof(glm::vec3) * normals.size();

        m_offsetUv0s = ALIGN_BYTES_4(m_offsetNormals + sizeNormals);
        VkDeviceSize sizeUv0s = sizeof(glm::vec2) * uv0s.size();

        m_buffer = new VulkanBuffer(m_logicalDevice);
        m_buffer->createBuffer(m_offsetUv0s + sizeUv0s,
                               VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
                                   | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                               VK_SHARING_MODE_EXCLUSIVE);

        m_buffer->uploadBuffer(
            [&](void* destBuffer, VkDeviceSize destBufferSize)
            {
                memcpy(((uint8*)destBuffer + m_offsetIndices), indices.data(), sizeIndices);
                memcpy(((uint8*)destBuffer + m_offsetPositions), positions.data(), sizePositions);
                memcpy(((uint8*)destBuffer + m_offsetNormals), normals.data(), sizeNormals);
                memcpy(((uint8*)destBuffer + m_offsetUv0s), uv0s.data(), sizeUv0s);
            });

        m_vertexInputMask = VulkanPipeline::InputLocation_Position | VulkanPipeline::InputLocation_Normal
                            | VulkanPipeline::InputLocation_Uv0;
        m_topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        return true;
    }

    bool TriangleListGeometry::uploadData(const void* data,
                                          VkDeviceSize dataSize,
                                          uint32 numVertices,
                                          uint32 numIndices,
                                          VkDeviceSize offsetPosition,
                                          VkDeviceSize offsetNormal,
                                          VkDeviceSize offsetUv0,
                                          VkDeviceSize offsetIndices)
    {
        destroy();

        m_numVertices = numVertices;
        m_numIndices = numIndices;

        m_offsetPositions = offsetPosition;
        m_offsetNormals = offsetNormal;
        m_offsetUv0s = offsetUv0;
        m_offsetIndices = offsetIndices;

        m_buffer = new VulkanBuffer(m_logicalDevice);
        m_buffer->createBuffer(dataSize,
                               VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
                                   | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                               VK_SHARING_MODE_EXCLUSIVE);

        m_buffer->uploadBuffer(data, dataSize);

        m_vertexInputMask = VulkanPipeline::InputLocation_Position | VulkanPipeline::InputLocation_Normal
                            | VulkanPipeline::InputLocation_Uv0;

        return true;
    }

    VkDeviceSize TriangleListGeometry::getVertexInputOffset(uint32 inputMask) const
    {
        if (inputMask == VulkanPipeline::InputLocation_Position)
        {
            return m_offsetPositions;
        }
        else if (inputMask == VulkanPipeline::InputLocation_Normal)
        {
            return m_offsetNormals;
        }
        else if (inputMask == VulkanPipeline::InputLocation_Uv0)
        {
            return m_offsetUv0s;
        }

        return 0;
    }

    void TriangleListGeometry::destroy()
    {
        SAFE_DELETE(m_buffer);

        m_offsetPositions = 0;
        m_offsetNormals = 0;
        m_offsetUv0s = 0;
        m_offsetIndices = 0;
    }
} // namespace blink