/*********************************************************************
 * \file   DynamicIntegrateSystem.h
 * \brief  
 * 
 * \author zjhlogo
 * \date   2/13 2022
 *********************************************************************/
#pragma once

#include <core/ILogicalSystem.h>

namespace blink
{
    class DynamicIntegrateSystem final : public ILogicalSystem
    {
    public:
        bool initialize() override;
        void terminate() override;
    };
}
