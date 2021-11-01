/**

    @file      Mesh.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <foundation/BaseTypesGlm.h>
#include <vulkan/vulkan.h>

#include <vector>

NS_BEGIN

class VulkanLogicalDevice;
class VulkanCommandPool;
class VulkanCommandBuffer;
class VulkanBuffer;

class Mesh
{
public:
    Mesh(VulkanLogicalDevice& logicalDevice, VulkanCommandPool& commandPool);
    ~Mesh();

    bool loadFromFile(const tstring& filePath);
    bool uploadData(const void* vertexData,
                    uint32 vertexDataSize,
                    const uint16* indexData,
                    uint32 numIndices);

    void bindMesh(VulkanCommandBuffer& commandBuffer);

    VulkanBuffer* getVertexBuffer() { return m_vertexBuffer; };
    VulkanBuffer* getIndexBuffer() { return m_indexBuffer; };
    uint32 getNumIndices() { return m_numIndices; };

private:
    void destroy();

private:
    VulkanLogicalDevice& m_logicalDevice;
    VulkanCommandPool& m_commandPool;

    VulkanBuffer* m_vertexBuffer{};
    VulkanBuffer* m_indexBuffer{};
    uint32_t m_numIndices{};
};

NS_END
