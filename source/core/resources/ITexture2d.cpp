
/*********************************************************************
 * \file   ITexture2d.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#include "ITexture2d.h"

#include <core/modules/IResModule.h>

namespace blink
{
    void ITexture2d::release()
    {
        //
        getResModule()->releaseTexture2d(this);
    }
} // namespace blink
