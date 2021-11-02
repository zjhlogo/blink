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
#include <vulkan/vulkan.h>

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
        VkDeviceSize offset = 0;
        VkBuffer vertexBuffer = *getVertexBuffer();
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, &offset);

        VkBuffer indicesBuffer = *getIndexBuffer();
        vkCmdBindIndexBuffer(commandBuffer, indicesBuffer, 0, VK_INDEX_TYPE_UINT16);
    }

    void IGeometry::destroy()
    {
        SAFE_DELETE(m_indexBuffer);
        SAFE_DELETE(m_vertexBuffer);
    }

    bool IGeometry::uploadData(const void* vertexData, size_t numVertices, size_t vertexSize, const uint16* indexData, size_t numIndices)
    {
        destroy();

        VkDeviceSize vertexBufferSize = numVertices * vertexSize;
        m_vertexBuffer = new VulkanBuffer(m_logicalDevice);
        if (!m_vertexBuffer->createBufferAndUpload(vertexData,
                                                   vertexBufferSize,
                                                   VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                                   VK_SHARING_MODE_EXCLUSIVE,
                                                   m_commandPool))
        {
            return false;
        }
        m_numVertices = static_cast<uint32>(numVertices);

        m_indexBuffer = new VulkanBuffer(m_logicalDevice);
        if (!m_indexBuffer->createBufferAndUpload(indexData,
                                                  sizeof(uint16) * numIndices,
                                                  VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                                                  VK_SHARING_MODE_EXCLUSIVE,
                                                  m_commandPool))
        {
            return false;
        }
        m_numIndices = static_cast<uint32>(numIndices);

        return true;
    }
} // namespace blink
