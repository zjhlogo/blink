
/*********************************************************************
 * \file   VulkanGeometryLineList.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/15/2022
 *********************************************************************/
#include "VulkanGeometryLineList.h"

#include <render_vulkan/VulkanBuffer.h>
#include <render_vulkan/VulkanPipeline.h>

namespace blink
{
    VulkanGeometryLineList::VulkanGeometryLineList(VulkanLogicalDevice& logicalDevice)
        : m_logicalDevice(logicalDevice)
    {
        //
    }

    VulkanGeometryLineList::~VulkanGeometryLineList()
    {
        //
        destroy();
    }

    bool VulkanGeometryLineList::uploadData(const std::vector<uint16>& indices,
                                            const std::vector<glm::vec3>& positions,
                                            const std::vector<Color>& colors)
    {
        // swap buffer
        m_offsetIndices = 0;
        m_numIndices = static_cast<uint32>(indices.size());
        VkDeviceSize sizeIndices = sizeof(uint16) * m_numIndices;

        m_offsetPositions = ALIGN_BYTES_4(m_offsetIndices + sizeIndices);
        VkDeviceSize sizePositions = sizeof(glm::vec3) * positions.size();

        m_offsetColors = ALIGN_BYTES_4(m_offsetPositions + sizePositions);
        VkDeviceSize sizeColors = sizeof(Color) * colors.size();

        auto buffer = swapBuffer();
        assert(buffer->getBufferSize() >= (m_offsetColors + sizeColors));

        buffer->uploadBuffer(
            [&](void* destBuffer, VkDeviceSize destBufferSize)
            {
                memcpy(((uint8*)destBuffer + m_offsetIndices), indices.data(), sizeIndices);
                memcpy(((uint8*)destBuffer + m_offsetPositions), positions.data(), sizePositions);
                memcpy(((uint8*)destBuffer + m_offsetColors), colors.data(), sizeColors);
            });

        m_vertexInputMask = VulkanPipeline::InputLocation_Position | VulkanPipeline::InputLocation_Color;
        m_topology = PrimitiveTopology::LineList;

        return true;
    }

    bool VulkanGeometryLineList::uploadData(const std::vector<uint16>& indices,
                                            const std::vector<glm::vec3>& positions,
                                            const std::vector<glm::vec3>& normals,
                                            const std::vector<glm::vec2>& uv0s)
    {
        assert(false && "not support");
        return false;
    }

    bool VulkanGeometryLineList::uploadData(const void* data,
                                            std::size_t dataSize,
                                            uint32 numVertices,
                                            uint32 numIndices,
                                            std::size_t offsetPosition,
                                            std::size_t offsetNormal,
                                            std::size_t offsetUv0,
                                            std::size_t offsetIndices)
    {
        assert(false && "not support");
        return false;
    }

    VkDeviceSize VulkanGeometryLineList::getVertexInputOffset(uint32 inputMask) const
    {
        if (inputMask == VulkanPipeline::InputLocation_Position)
        {
            return m_offsetPositions;
        }
        else if (inputMask == VulkanPipeline::InputLocation_Color)
        {
            return m_offsetColors;
        }

        return 0;
    }

    void VulkanGeometryLineList::destroy()
    {
        for (auto buffer : m_bufferList)
        {
            SAFE_DELETE(buffer);
        }

        m_currentBuffer = 0;
        m_offsetPositions = 0;
        m_offsetColors = 0;
        m_offsetIndices = 0;
    }

    VulkanBuffer* VulkanGeometryLineList::swapBuffer()
    {
        m_currentBuffer = (m_currentBuffer + 1) % BUFFER_COUNT;

        auto buffer = m_bufferList[m_currentBuffer];
        if (buffer) return buffer;

        buffer = new VulkanBuffer(m_logicalDevice);
        buffer->createBuffer(DEFAULT_BUFFER_SIZE,
                             VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
                                 | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                             VK_SHARING_MODE_EXCLUSIVE);
        m_bufferList[m_currentBuffer] = buffer;

        return buffer;
    }
} // namespace blink
