
/*********************************************************************
 * \file   PrefabInitializeSystem.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   2/19 2022
 *********************************************************************/
#include "PrefabInitializeSystem.h"

#include <core/components/Components.h>
#include <physics/components/Components.h>

PrefabInitializeSystem::PrefabInitializeSystem()
{
}

bool PrefabInitializeSystem::initialize(flecs::world& world)
{
    // rigid body prefab
    prefabRigidBody = world.prefab("RigidBody")
                          .set_override<blink::Position>({glm::zero<glm::vec3>()})
                          .set_override<blink::Rotation>({glm::identity<glm::quat>()})
                          .set_override<blink::PhysicsVelocity>({glm::zero<glm::vec3>(), glm::zero<glm::vec3>()})
                          .set_override<blink::PhysicsMass>(blink::PhysicsMass(1.0f, glm::identity<glm::mat3>()))
                          .set_override<blink::PhysicsDamping>({0.9f, 0.9f})
                          .set_override<blink::PhysicsAccumulate>({glm::zero<glm::vec3>(), glm::zero<glm::vec3>()});

    return true;
}

void PrefabInitializeSystem::terminate(flecs::world& world)
{
    //
}
