/*********************************************************************
 * \file   IGeometry.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#include "IGeometry.h"
#include "../modules/IResourceModule.h"

namespace blink
{
    void IGeometry::release()
    {
        //
        getResourceModule()->releaseGeometry(this);
    }

    bool IGeometry::hasVertexAttrs(VertexAttrs requiredVertexAttrs) const
    {
        return (requiredVertexAttrs & m_vertexAttrs) == requiredVertexAttrs;
    }

} // namespace blink
