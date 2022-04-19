
/*********************************************************************
 * \file   SinglePendulumSystem.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   4/15 2022
 *********************************************************************/
#include "SinglePendulumSystem.h"
#include "../components/PendulumComponents.h"

#include <blink/systems/DebugLineSystem.h>
#include <core/EcsWorld.h>
#include <core/components/Components.h>
#include <physics/components/Components.h>

bool SinglePendulumSystem::initialize()
{
    m_ecsWorld->getWorld()
        .system<blink::PhysicsVelocity,
                blink::Position,
                const blink::PhysicsMass,
                const blink::PhysicsDamping,
                const SinglePendulum>()
        .each(
            [](flecs::entity e,
               blink::PhysicsVelocity& pv,
               blink::Position& pos,
               const blink::PhysicsMass& pm,
               const blink::PhysicsDamping& pd,
               const SinglePendulum& sp)
            {
                auto dt = e.delta_time();

                // calculate new applied velocity
                glm::vec3 addVelocity = dt * pm.inverseMass * sp.gravity;
                glm::vec3 velocity = pv.linearVelocity + addVelocity;

                // split velocity
                glm::vec3 normal = glm::normalize(pos.value - sp.anchorPoint);
                glm::vec3 velocityNormal = glm::dot(velocity, normal) * normal;
                glm::vec3 velocityTangent = velocity - velocityNormal;

                // update position
                pos.value += velocityTangent * dt;
                pos.value = sp.anchorPoint + glm::normalize(pos.value - sp.anchorPoint) * sp.length;

                // update velocity
                pv.linearVelocity = velocityTangent * glm::pow(pd.linearDamping, dt);
            });

    return true;
}

void SinglePendulumSystem::terminate()
{
    //
}

void SinglePendulumSystem::framePostUpdate()
{
    static auto pendulumQuery = m_ecsWorld->getWorld().query_builder<const blink::Position, const SinglePendulum>().build();

    auto debugLineSystem = m_ecsWorld->findSystem<blink::DebugLineSystem>();

    pendulumQuery.each(
        [&](flecs::entity e, const blink::Position& pos, const SinglePendulum& pendulum)
        {
            // add debug line
            debugLineSystem->addLine(pendulum.anchorPoint, pos.value, blink::Color::RED);
        });
}
