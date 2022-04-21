
/*********************************************************************
 * \file   ITexture2d.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#include "ITexture2d.h"
#include "../modules/IResourceModule.h"

namespace blink
{
    void ITexture2d::release()
    {
        //
        getResourceModule()->releaseTexture2d(this);
    }
} // namespace blink
