
/*********************************************************************
 * \file   IGeometry.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/15/2022
 *********************************************************************/
#include "IGeometry.h"

#include <blink/resources/ResourceMgr.h>
#include <render_vulkan/VulkanBuffer.h>

namespace blink
{
    IGeometry::IGeometry(VulkanLogicalDevice& logicalDevice)
        : m_logicalDevice(logicalDevice)
    {
        //
    }

    IGeometry::~IGeometry()
    {
        m_numVertices = 0;
        m_numIndices = 0;
        m_vertexInputMask = 0;
    }

    void IGeometry::release()
    {
        //
        ResourceMgr::getInstance().releaseGeometry(this);
    }

    bool IGeometry::checkInputMask(uint32 requireInputMask) const
    {
        return (requireInputMask & m_vertexInputMask) == requireInputMask;
    }
} // namespace blink
