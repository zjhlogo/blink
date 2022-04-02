
/*********************************************************************
 * \file   BeginPhysicsSimulationSystem.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   2/19 2022
 *********************************************************************/
#include "BeginPhysicsSimulationSystem.h"
#include "../components/Components.h"

#include <core/components/Components.h>

namespace blink
{
    bool BeginPhysicsSimulationSystem::initialize(flecs::world& world)
    {
        // TODO: 
        return true;
    }

    void BeginPhysicsSimulationSystem::terminate(flecs::world& world)
    {
        //
    }
} // namespace blink
