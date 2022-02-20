
/*********************************************************************
 * \file   DynamicIntegrateSystem.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   2/13 2022
 *********************************************************************/
#include "DynamicIntegrateSystem.h"
#include "../component/Components.h"

#include <core/component/Components.h>

namespace blink
{
    bool DynamicIntegrateSystem::initialize(flecs::world& world)
    {
        world.system<PhysicsVelocity, Position, Rotation, const PhysicsMass, const PhysicsDamping, const PhysicsAccumulate>("calculate physics velocity")
            .each(
                [](flecs::entity e,
                   PhysicsVelocity& pv,
                   Position& pos,
                   Rotation& rot,
                   const PhysicsMass& pm,
                   const PhysicsDamping& pd,
                   const PhysicsAccumulate& pa)
                {
                    // calculate linear acceleration from force accumulate
                    auto linearAcceleration = pa.forceAccum * pm.inverseMass;

                    // calculate anguar acceleration from torque accumulate
                    auto matRot = glm::mat3_cast(rot.value);
                    auto inverseInertiaTensorWorld = matRot * pm.inverseInertiaTensor;
                    auto angularAcceleration = inverseInertiaTensorWorld * pa.torqueAccum;

                    auto deltaTime = e.delta_time();

                    // update linear velocity from both acceleration and impulse
                    pv.linearVelocity += linearAcceleration * deltaTime;
                    pv.angularVelocity += angularAcceleration * deltaTime;

                    // impose drag
                    pv.linearVelocity *= glm::pow(pd.linearDamping, deltaTime);
                    pv.angularVelocity *= glm::pow(pd.angularDamping, deltaTime);

                    // update linear and angular position
                    pos.value += pv.linearVelocity * deltaTime;
                    rot.value += (glm::quat(pv.angularVelocity * deltaTime) * rot.value * 0.5f);
                    rot.value = glm::normalize(rot.value);

                    // impose drag
                    pv.linearVelocity *= glm::pow(pd.linearDamping, deltaTime);
                    pv.angularVelocity *= glm::pow(pd.angularDamping, deltaTime);
                });

        return true;
    }

    void DynamicIntegrateSystem::terminate(flecs::world& world)
    {
        //
    }
} // namespace blink
