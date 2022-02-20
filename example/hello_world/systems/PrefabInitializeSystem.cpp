
/*********************************************************************
 * \file   PrefabInitializeSystem.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   2/19 2022
 *********************************************************************/
#include "PrefabInitializeSystem.h"

#include <core/component/Components.h>
#include <physics/component/Components.h>

PrefabInitializeSystem::PrefabInitializeSystem()
{
}

bool PrefabInitializeSystem::initialize(flecs::world& world)
{
    // rigid body prefab
    prefabRigidBody = world.prefab("RigidBody")
                          .set<blink::Position>({glm::zero<glm::vec3>()})
                          .set<blink::Rotation>({glm::identity<glm::quat>()})
                          .set<blink::PhysicsVelocity>({glm::zero<glm::vec3>(), glm::zero<glm::vec3>()})
                          .emplace<blink::PhysicsMass>(1.0f, glm::identity<glm::mat3>())
                          .set<blink::PhysicsDamping>({0.9f, 0.9f})
                          .set<blink::PhysicsAccumulate>({glm::zero<glm::vec3>(), glm::zero<glm::vec3>()});

    return true;
}

void PrefabInitializeSystem::terminate(flecs::world& world)
{
    //
}
