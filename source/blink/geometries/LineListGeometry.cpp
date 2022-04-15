
/*********************************************************************
 * \file   LineListGeometry.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/15/2022
 *********************************************************************/
#include "LineListGeometry.h"

#include <blink/resources/ResourceMgr.h>
#include <render_vulkan/VulkanBuffer.h>
#include <render_vulkan/VulkanPipeline.h>

namespace blink
{
    LineListGeometry::LineListGeometry(VulkanLogicalDevice& logicalDevice)
        : IGeometry(logicalDevice)
    {
        //
    }

    LineListGeometry::~LineListGeometry()
    {
        //
        destroy();
    }

    bool LineListGeometry::uploadData(const std::vector<uint16>& indices, const std::vector<glm::vec3>& positions)
    {
        destroy();

        m_offsetIndices = 0;
        m_numIndices = static_cast<uint32>(indices.size());
        VkDeviceSize sizeIndices = sizeof(uint16) * m_numIndices;

        m_offsetPositions = ALIGN_BYTES_4(m_offsetIndices + sizeIndices);
        VkDeviceSize sizePositions = sizeof(glm::vec3) * positions.size();

        m_buffer = new VulkanBuffer(m_logicalDevice);
        m_buffer->createBuffer(m_offsetPositions + sizePositions,
                               VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
                                   | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                               VK_SHARING_MODE_EXCLUSIVE);

        m_buffer->uploadBuffer(
            [&](void* destBuffer, VkDeviceSize destBufferSize)
            {
                memcpy(((uint8*)destBuffer + m_offsetIndices), indices.data(), sizeIndices);
                memcpy(((uint8*)destBuffer + m_offsetPositions), positions.data(), sizePositions);
            });

        m_vertexInputMask = VulkanPipeline::InputLocation_Position;

        return true;
    }

    VkDeviceSize LineListGeometry::getVertexInputOffset(uint32 inputMask) const
    {
        if (inputMask == VulkanPipeline::InputLocation_Position)
        {
            return m_offsetPositions;
        }

        return 0;
    }

    void LineListGeometry::destroy()
    {
        SAFE_DELETE(m_buffer);

        m_offsetPositions = 0;
        m_offsetIndices = 0;
    }
} // namespace blink
