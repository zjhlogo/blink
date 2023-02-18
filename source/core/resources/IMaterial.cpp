/*********************************************************************
 * \file   IMaterial.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#include "IMaterial.h"
#include "../modules/IResModule.h"

namespace blink
{
    void IMaterial::release()
    {
        //
        getResModule()->releaseMaterial(this);
    }
} // namespace blink
