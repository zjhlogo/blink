
/*********************************************************************
 * \file   IGeometry.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#pragma once

#include "IResource.h"

namespace blink
{
    enum class PrimitiveTopology
    {
        LineList,
        TriangleList
    };

    class IGeometry : public IResource
    {
        friend class IResourceModule;

    public:
        virtual void release() override;

        uint32 getNumVertices() const { return m_numVertices; };
        uint32 getNumIndices() const { return m_numIndices; };

        bool checkInputMask(uint32 requireInputMask) const;
        PrimitiveTopology getTopology() const { return m_topology; }

    protected:
        uint32 m_numVertices{};
        uint32 m_numIndices{};

        uint32 m_vertexInputMask{};
        PrimitiveTopology m_topology{PrimitiveTopology::TriangleList};
    };
} // namespace blink
