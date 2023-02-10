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

    class VulkanGeometryLineList final : public VulkanGeometry
    {
        static constexpr int BUFFER_COUNT = 2;
        static constexpr VkDeviceSize DEFAULT_BUFFER_SIZE = 32ull * 1024ull;

    public:
        explicit VulkanGeometryLineList(VulkanLogicalDevice& logicalDevice);
        ~VulkanGeometryLineList() override;

        VulkanGeometryLineList(const VulkanGeometryLineList& lineList) = delete;
        VulkanGeometryLineList(VulkanGeometryLineList&& lineList) = delete;
        VulkanGeometryLineList& operator=(const VulkanGeometryLineList& lineList) = delete;
        VulkanGeometryLineList& operator=(VulkanGeometryLineList&& lineList) = delete;

        bool uploadData(const std::vector<uint16_t>& indices, const std::vector<glm::vec3>& positions, const std::vector<Color>& colors) override;

        bool uploadData(const std::vector<uint16_t>& indices,
                        const std::vector<glm::vec3>& positions,
                        const std::vector<glm::vec3>& normals,
                        const std::vector<glm::vec2>& uvs) override;

        bool uploadData(const void* data,
                        std::size_t dataSize,
                        uint32_t numVertices,
                        uint32_t numIndices,
                        std::size_t offsetPosition,
                        std::size_t offsetNormal,
                        std::size_t offsetUv0,
                        std::size_t offsetIndices) override;

        VulkanBuffer* getVulkanBuffer() const override { return m_bufferList[m_currentBuffer]; }
        VkDeviceSize getVertexInputOffset(VertexAttrs vertexAttrs) const override;
        VkDeviceSize getIndicesOffset() const override { return m_offsetIndices; }

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
