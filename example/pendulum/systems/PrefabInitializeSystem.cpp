/*********************************************************************
 * \file   PrefabInitializeSystem.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   2/19 2022
 *********************************************************************/
#include "PrefabInitializeSystem.h"

#include <blink/components/Components.h>
#include <core/EcsWorld.h>
#include <core/components/Components.h>
#include <physics/components/Components.h>

bool PrefabInitializeSystem::initialize()
{
    auto& world = m_ecsWorld->getWorld();

    world.component<glm::vec3>().member<float>("x").member<float>("y").member<float>("z");
    world.component<glm::quat>().member<float>("x").member<float>("y").member<float>("z").member<float>("w");

    world.component<blink::Position>().member<glm::vec3>("value");
    world.component<blink::Rotation>().member<glm::quat>("value");
    world.component<blink::PhysicsVelocity>().member<glm::vec3>("linearVelocity").member<glm::vec3>("angularVelocity");
    world.component<blink::PhysicsMass>().member<float>("inverseMass");
    world.component<blink::PhysicsDamping>().member<float>("linearDamping").member<float>("angularDamping");
    world.component<blink::PhysicsAccumulate>().member<glm::vec3>("forceAccum").member<glm::vec3>("torqueAccum");

    // rigid body prefab
    prefabRigidBody = world.prefab("rigid_body")
                          .set_override<blink::Position>({glm::zero<glm::vec3>()})
                          .set_override<blink::Rotation>({glm::identity<glm::quat>()})
                          .set_override<blink::PhysicsVelocity>({glm::zero<glm::vec3>(), glm::zero<glm::vec3>()})
                          .set_override<blink::PhysicsMass>(blink::PhysicsMass(1.0f, glm::identity<glm::mat3>()))
                          .set_override<blink::PhysicsDamping>({0.9f, 0.9f})
                          .set_override<blink::PhysicsAccumulate>({glm::zero<glm::vec3>(), glm::zero<glm::vec3>()})
                          .set_override<blink::Renderable>({blink::RenderLayers::NORMAL});

    return true;
}

void PrefabInitializeSystem::terminate()
{
    //
}
