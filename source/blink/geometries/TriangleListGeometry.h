
/*********************************************************************
 * \file   TriangleListGeometry.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/15/2022
 *********************************************************************/
#pragma once

#include "IGeometry.h"

#include <vector>

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanCommandBuffer;
    class VulkanBuffer;
    class ResourceMgr;

    class TriangleListGeometry : public IGeometry
    {
        friend ResourceMgr;

    public:
        bool uploadData(const std::vector<uint16>& indices,
                        const std::vector<glm::vec3>& positions,
                        const std::vector<glm::vec3>& normals,
                        const std::vector<glm::vec2>& uv0s);

        bool uploadData(const void* data,
                        VkDeviceSize dataSize,
                        uint32 numVertices,
                        uint32 numIndices,
                        VkDeviceSize offsetPosition,
                        VkDeviceSize offsetNormal,
                        VkDeviceSize offsetUv0,
                        VkDeviceSize offsetIndices);

        virtual VulkanBuffer* getVulkanBuffer() const override { return m_buffer; }
        virtual VkDeviceSize getVertexInputOffset(uint32 inputMask) const override;
        virtual VkDeviceSize getIndicesOffset() const override { return m_offsetIndices; }

    protected:
        TriangleListGeometry(VulkanLogicalDevice& logicalDevice);
        virtual ~TriangleListGeometry();

        void destroy();

    protected:
        VulkanBuffer* m_buffer{};

        VkDeviceSize m_offsetPositions{};
        VkDeviceSize m_offsetNormals{};
        VkDeviceSize m_offsetUv0s{};
        VkDeviceSize m_offsetIndices{};
    };
} // namespace blink
