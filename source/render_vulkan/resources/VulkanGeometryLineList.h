
/*********************************************************************
 * \file   VulkanGeometryLineList.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/15/2022
 *********************************************************************/
#pragma once

#include "VulkanGeometry.h"

namespace blink
{
    class VulkanLogicalDevice;

    class VulkanGeometryLineList : public VulkanGeometry
    {
        static const int BUFFER_COUNT = 2;
        static const VkDeviceSize DEFAULT_BUFFER_SIZE = 32 * 1024;

    public:
        VulkanGeometryLineList(VulkanLogicalDevice& logicalDevice);
        virtual ~VulkanGeometryLineList();

        virtual bool uploadData(const std::vector<uint16>& indices,
                                const std::vector<glm::vec3>& positions,
                                const std::vector<Color>& colors) override;

        virtual bool uploadData(const std::vector<uint16>& indices,
                                const std::vector<glm::vec3>& positions,
                                const std::vector<glm::vec3>& normals,
                                const std::vector<glm::vec2>& uv0s) override;

        virtual bool uploadData(const void* data,
                                std::size_t dataSize,
                                uint32 numVertices,
                                uint32 numIndices,
                                std::size_t offsetPosition,
                                std::size_t offsetNormal,
                                std::size_t offsetUv0,
                                std::size_t offsetIndices) override;

        virtual VulkanBuffer* getVulkanBuffer() const override { return m_bufferList[m_currentBuffer]; }
        virtual VkDeviceSize getVertexInputOffset(uint32 inputMask) const override;
        virtual VkDeviceSize getIndicesOffset() const override { return m_offsetIndices; }

    protected:
        void destroy();
        VulkanBuffer* swapBuffer();

    protected:
        VulkanLogicalDevice& m_logicalDevice;
        int m_currentBuffer{};
        VulkanBuffer* m_bufferList[BUFFER_COUNT]{};

        VkDeviceSize m_offsetPositions{};
        VkDeviceSize m_offsetColors{};
        VkDeviceSize m_offsetIndices{};
    };
} // namespace blink
