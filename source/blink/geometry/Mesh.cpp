/**

    @file      Mesh.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/

#include "Mesh.h"

#include <foundation/Log.h>
#include <render_vulkan/Types.h>
#include <render_vulkan/VulkanBuffer.h>
#include <render_vulkan/VulkanCommandBuffer.h>

#include <unordered_map>

#define TINYOBJLOADER_IMPLEMENTATION
#include <render_vulkan/utils/tiny_obj_loader.h>

NS_BEGIN

Mesh::Mesh(VulkanLogicalDevice& logicalDevice, VulkanCommandPool& commandPool)
    : m_logicalDevice(logicalDevice)
    , m_commandPool(commandPool)
{
}

Mesh::~Mesh()
{
    destroy();
}

bool Mesh::loadFromFile(const tstring& filePath)
{
    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(filePath))
    {
        LOGE(reader.Error());
        return false;
    }

    if (!reader.Warning().empty())
    {
        LOGW(reader.Warning());
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    std::vector<VertexPosColorUv1> vertices;
    std::vector<uint16> indices;
    std::unordered_map<VertexPosColorUv1, uint16> uniqueVertices;

    // Loop over shapes
    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            VertexPosColorUv1 vertex{{
                                         attrib.vertices[3 * index.vertex_index + 0],
                                         attrib.vertices[3 * index.vertex_index + 1],
                                         attrib.vertices[3 * index.vertex_index + 2],
                                     },
                                     {
                                         1.0f,
                                         1.0f,
                                         1.0f,
                                     },
                                     {
                                         attrib.texcoords[2 * index.texcoord_index + 0],
                                         1.0f - attrib.texcoords[2 * index.texcoord_index + 1],
                                     }};

            if (uniqueVertices.count(vertex) == 0)
            {
                uniqueVertices[vertex] = static_cast<uint16>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }

    return uploadData(vertices.data(),
                      static_cast<uint32>(sizeof(VertexPosColorUv1) * vertices.size()),
                      indices.data(),
                      static_cast<uint16>(indices.size()));
}

bool Mesh::uploadData(const void* vertexData,
                      uint32 vertexDataSize,
                      const uint16* indexData,
                      uint32 numIndices)
{
    destroy();

    m_vertexBuffer = new VulkanBuffer(m_logicalDevice);
    if (!m_vertexBuffer->createBufferAndUpload(vertexData,
                                               vertexDataSize,
                                               VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                               VK_SHARING_MODE_EXCLUSIVE,
                                               m_commandPool))
    {
        return false;
    }

    m_indexBuffer = new VulkanBuffer(m_logicalDevice);
    if (!m_indexBuffer->createBufferAndUpload(indexData,
                                              sizeof(uint16) * numIndices,
                                              VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                                              VK_SHARING_MODE_EXCLUSIVE,
                                              m_commandPool))
    {
        return false;
    }

    m_numIndices = numIndices;

    return true;
}

void Mesh::bindMesh(VulkanCommandBuffer& commandBuffer)
{
    VkDeviceSize offset = 0;
    VkBuffer vertexBuffer = *m_vertexBuffer;
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, &offset);

    vkCmdBindIndexBuffer(commandBuffer, *m_indexBuffer, 0, VK_INDEX_TYPE_UINT16);
}

void Mesh::destroy()
{
    SAFE_DELETE(m_indexBuffer);
    SAFE_DELETE(m_vertexBuffer);
}

NS_END
