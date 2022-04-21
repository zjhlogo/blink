
/*********************************************************************
 * \file   VulkanGeometryLineList.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/15/2022
 *********************************************************************/
#pragma once

#include <core/resources/IGeometryLineList.h>
#include <vulkan/vulkan_core.h>

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanCommandBuffer;
    class VulkanBuffer;
    class ResourceMgr;

    class VulkanGeometryLineList : public IGeometryLineList
    {
        friend ResourceMgr;

        static const int BUFFER_COUNT = 2;
        static const VkDeviceSize DEFAULT_BUFFER_SIZE = 32 * 1024;

    public:
        virtual bool uploadData(const std::vector<uint16>& indices,
                                const std::vector<glm::vec3>& positions,
                                const std::vector<Color>& colors) override;

        VulkanBuffer* getVulkanBuffer() const { return m_bufferList[m_currentBuffer]; }
        VkDeviceSize getVertexInputOffset(uint32 inputMask) const;
        VkDeviceSize getIndicesOffset() const { return m_offsetIndices; }

    protected:
        VulkanGeometryLineList(VulkanLogicalDevice& logicalDevice);
        virtual ~VulkanGeometryLineList();

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
