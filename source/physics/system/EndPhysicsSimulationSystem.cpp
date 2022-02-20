
/*********************************************************************
 * \file   EndPhysicsSimulationSystem.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   2/19 2022
 *********************************************************************/
#include "EndPhysicsSimulationSystem.h"
#include "../component/Components.h"

#include <core/component/Components.h>

namespace blink
{
    bool EndPhysicsSimulationSystem::initialize(flecs::world& world)
    {
        world.system<PhysicsAccumulate>("cleanup physics accumulate")
            .each(
                [](flecs::entity e, PhysicsAccumulate& pa)
                {
                    pa.forceAccum = glm::zero<glm::vec3>();
                    pa.torqueAccum = glm::zero<glm::vec3>();
                });

        return true;
    }

    void EndPhysicsSimulationSystem::terminate(flecs::world& world)
    {
        //
    }
} // namespace blink
