
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

    public:
        bool uploadData(const std::vector<uint16>& indices, const std::vector<glm::vec3>& positions);

        virtual VulkanBuffer* getVulkanBuffer() const override { return m_buffer; }
        virtual VkDeviceSize getVertexInputOffset(uint32 inputMask) const override;
        virtual VkDeviceSize getIndicesOffset() const override { return m_offsetIndices; }

    protected:
        LineListGeometry(VulkanLogicalDevice& logicalDevice);
        virtual ~LineListGeometry();

        void destroy();

    protected:
        VulkanBuffer* m_buffer{};

        VkDeviceSize m_offsetPositions{};
        VkDeviceSize m_offsetIndices{};
    };
} // namespace blink
