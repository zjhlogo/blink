
/*********************************************************************
 * \file   IGeometry.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/15/2022
 *********************************************************************/
#pragma once

#include "../resources/IResource.h"

#include <foundation/BaseTypesGlm.h>
#include <vulkan/vulkan.h>

#include <vector>

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanBuffer;

    class IGeometry : public IResource
    {
        friend ResourceMgr;

    public:
        virtual void release() override;

        uint32 getNumVertices() const { return m_numVertices; };
        uint32 getNumIndices() const { return m_numIndices; };

        virtual VulkanBuffer* getVulkanBuffer() const = 0;
        virtual VkDeviceSize getVertexInputOffset(uint32 inputMask) const = 0;
        virtual VkDeviceSize getIndicesOffset() const = 0;

        bool checkInputMask(uint32 requireInputMask) const;
        VkPrimitiveTopology getTopology() const { return m_topology; }

    protected:
        IGeometry(VulkanLogicalDevice& logicalDevice);
        virtual ~IGeometry();

    protected:
        VulkanLogicalDevice& m_logicalDevice;

        uint32 m_numVertices{};
        uint32 m_numIndices{};

        uint32 m_vertexInputMask{};
        VkPrimitiveTopology m_topology{VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST};
    };
} // namespace blink
