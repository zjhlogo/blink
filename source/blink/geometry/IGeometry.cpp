/**

    @file      IGeometry.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      2.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/

#include "IGeometry.h"

#include <render_vulkan/VulkanBuffer.h>
#include <render_vulkan/VulkanCommandBuffer.h>

namespace blink
{
    IGeometry::IGeometry(VulkanLogicalDevice& logicalDevice, VulkanCommandPool& commandPool)
        : m_logicalDevice(logicalDevice)
        , m_commandPool(commandPool)
    {
    }

    IGeometry::~IGeometry() { destroy(); }

    void IGeometry::bindBuffer(VulkanCommandBuffer& commandBuffer)
    {
        VkBuffer buffer = *m_buffer;

        vkCmdBindIndexBuffer(commandBuffer, buffer, m_offsetIndices, VK_INDEX_TYPE_UINT16);

        vkCmdBindVertexBuffers(commandBuffer, 0, 1, &buffer, &m_offsetPositions);
        vkCmdBindVertexBuffers(commandBuffer, 1, 1, &buffer, &m_offsetNormals);
        vkCmdBindVertexBuffers(commandBuffer, 2, 1, &buffer, &m_offsetUv0s);
    }

    void IGeometry::destroy()
    {
        SAFE_DELETE(m_buffer);

        m_offsetPositions = 0;
        m_sizePositions = 0;

        m_offsetNormals = 0;
        m_sizeNormals = 0;

        m_offsetUv0s = 0;
        m_sizeUv0s = 0;

        m_offsetIndices = 0;
        m_sizeIndices = 0;
        m_numIndices = 0;
    }

    static VkDeviceSize align4Bytes(VkDeviceSize size) { return ((size + 3) >> 2) << 2; }

    bool IGeometry::uploadData(const std::vector<uint16>& indices,
                               const std::vector<glm::vec3>& positions,
                               const std::vector<glm::vec3>& normals,
                               const std::vector<glm::vec2>& uv0s)
    {
        destroy();

        m_offsetIndices = 0;
        m_numIndices = static_cast<uint32>(indices.size());
        m_sizeIndices = sizeof(uint16) * m_numIndices;

        m_offsetPositions = align4Bytes(m_offsetIndices + m_sizeIndices);
        m_sizePositions = sizeof(glm::vec3) * positions.size();

        m_offsetNormals = align4Bytes(m_offsetPositions + m_sizePositions);
        m_sizeNormals = sizeof(glm::vec3) * normals.size();

        m_offsetUv0s = align4Bytes(m_offsetNormals + m_sizeNormals);
        m_sizeUv0s = sizeof(glm::vec2) * uv0s.size();

        m_buffer = new VulkanBuffer(m_logicalDevice);
        m_buffer->createBuffer(m_offsetUv0s + m_sizeUv0s,
                               VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                               VK_SHARING_MODE_EXCLUSIVE);

        m_buffer->uploadBuffer(m_commandPool,
                               [&](void* destBuffer, VkDeviceSize destBufferSize)
                               {
                                   memcpy(((uint8*)destBuffer + m_offsetIndices), indices.data(), m_sizeIndices);
                                   memcpy(((uint8*)destBuffer + m_offsetPositions), positions.data(), m_sizePositions);
                                   memcpy(((uint8*)destBuffer + m_offsetNormals), normals.data(), m_sizeNormals);
                                   memcpy(((uint8*)destBuffer + m_offsetUv0s), uv0s.data(), m_sizeUv0s);
                               });
        return true;
    }
} // namespace blink
