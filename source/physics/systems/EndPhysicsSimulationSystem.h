/*********************************************************************
 * \file   EndPhysicsSimulationSystem.h
 * \brief  
 * 
 * \author zjhlogo
 * \date   2/19 2022
 *********************************************************************/
#pragma once

#include <core/ILogicalSystem.h>

namespace blink
{
    class EndPhysicsSimulationSystem final : public ILogicalSystem
    {
    public:
        bool initialize() override;
        void terminate() override;
    };
}
