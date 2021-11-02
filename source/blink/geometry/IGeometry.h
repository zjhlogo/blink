/**

    @file      IGeometry.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      2.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <foundation/BaseTypes.h>

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanCommandPool;
    class VulkanCommandBuffer;
    class VulkanBuffer;

    class IGeometry
    {
    public:
        IGeometry(VulkanLogicalDevice& logicalDevice, VulkanCommandPool& commandPool);
        virtual ~IGeometry();

        VulkanBuffer* getVertexBuffer() { return m_vertexBuffer; };
        uint32 getNumVertices() { return m_numVertices; };

        VulkanBuffer* getIndexBuffer() { return m_indexBuffer; };
        uint32 getNumIndices() { return m_numIndices; };

        void bindBuffer(VulkanCommandBuffer& commandBuffer);

    protected:
        void destroy();

        bool uploadData(const void* vertexData, uint32 numVertices, uint32 vertexSize, const uint16* indexData, uint32 numIndices);

    protected:
        VulkanLogicalDevice& m_logicalDevice;
        VulkanCommandPool& m_commandPool;

        VulkanBuffer* m_vertexBuffer{};
        uint32 m_numVertices{};

        VulkanBuffer* m_indexBuffer{};
        uint32 m_numIndices{};
    };
} // namespace blink
