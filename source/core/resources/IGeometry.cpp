
/*********************************************************************
 * \file   IGeometry.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#include "IGeometry.h"
#include "../modules/IResModule.h"

namespace blink
{
    void IGeometry::release()
    {
        //
        getResModule()->releaseGeometry(this);
    }

    bool IGeometry::hasVertexAttrs(VertexAttrs requiredVertexAttrs) const
    {
        return (requiredVertexAttrs & m_vertexAttrs) == requiredVertexAttrs;
    }

} // namespace blink
