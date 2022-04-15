
/*********************************************************************
 * \file   LineListGeometry.h
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

    class LineListGeometry : public IGeometry
    {
        friend ResourceMgr;

        static const int BUFFER_COUNT = 2;
        static const VkDeviceSize DEFAULT_BUFFER_SIZE = 32 * 1024;

    public:
        bool uploadData(const std::vector<uint16>& indices, const std::vector<glm::vec3>& positions);

        virtual VulkanBuffer* getVulkanBuffer() const override { return m_bufferList[m_currentBuffer]; }
        virtual VkDeviceSize getVertexInputOffset(uint32 inputMask) const override;
        virtual VkDeviceSize getIndicesOffset() const override { return m_offsetIndices; }

    protected:
        LineListGeometry(VulkanLogicalDevice& logicalDevice);
        virtual ~LineListGeometry();

        void destroy();
        VulkanBuffer* swapBuffer();

    protected:
        int m_currentBuffer{};
        VulkanBuffer* m_bufferList[BUFFER_COUNT]{};

        VkDeviceSize m_offsetPositions{};
        VkDeviceSize m_offsetIndices{};
    };
} // namespace blink
