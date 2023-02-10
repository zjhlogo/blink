/*********************************************************************
 * \file   BeginPhysicsSimulationSystem.h
 * \brief  
 * 
 * \author zjhlogo
 * \date   2/19 2022
 *********************************************************************/
#pragma once

#include <core/ILogicalSystem.h>

namespace blink
{
    class BeginPhysicsSimulationSystem final : public ILogicalSystem
    {
    public:
        bool initialize() override;
        void terminate() override;
    };
}
