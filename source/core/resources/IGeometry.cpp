
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

    bool IGeometry::checkInputMask(uint32 requireInputMask) const
    {
        return (requireInputMask & m_vertexInputMask) == requireInputMask;
    }

} // namespace blink
