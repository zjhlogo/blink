/*********************************************************************
 * \file   EndPhysicsSimulationSystem.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   2/19 2022
 *********************************************************************/
#include "EndPhysicsSimulationSystem.h"
#include "../components/Components.h"

#include <core/EcsWorld.h>
#include <core/components/Components.h>

namespace blink
{
    bool EndPhysicsSimulationSystem::initialize()
    {
        m_ecsWorld->getWorld().system<PhysicsAccumulate>().each([](flecs::entity e, PhysicsAccumulate& pa) {
            pa.forceAccum = glm::zero<glm::vec3>();
            pa.torqueAccum = glm::zero<glm::vec3>();
        });

        return true;
    }

    void EndPhysicsSimulationSystem::terminate()
    {
        //
    }
} // namespace blink
