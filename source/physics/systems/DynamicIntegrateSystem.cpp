/*********************************************************************
 * \file   DynamicIntegrateSystem.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   2/13 2022
 *********************************************************************/
#include "DynamicIntegrateSystem.h"
#include "../components/Components.h"

#include <core/EcsWorld.h>
#include <core/components/Components.h>

namespace blink
{
    bool DynamicIntegrateSystem::initialize()
    {
        m_ecsWorld->getWorld().system<PhysicsVelocity, Position, Rotation, const PhysicsMass, const PhysicsDamping, const PhysicsAccumulate>().each(
            [](flecs::entity e,
               PhysicsVelocity& pv,
               Position& pos,
               Rotation& rot,
               const PhysicsMass& pm,
               const PhysicsDamping& pd,
               const PhysicsAccumulate& pa) {
                // calculate linear acceleration from force accumulate
                auto linearAcceleration = pa.forceAccum * pm.inverseMass;

                // calculate angular acceleration from torque accumulate
                auto matRot = glm::mat3_cast(rot.value);
                auto inverseInertiaTensorWorld = matRot * pm.inverseInertiaTensor;
                auto angularAcceleration = inverseInertiaTensorWorld * pa.torqueAccum;

                auto dt = e.delta_time();

                // update linear velocity from both acceleration and impulse
                pv.linearVelocity += linearAcceleration * dt;
                pv.angularVelocity += angularAcceleration * dt;

                // impose drag
                pv.linearVelocity *= glm::pow(pd.linearDamping, dt);
                pv.angularVelocity *= glm::pow(pd.angularDamping, dt);

                // update linear and angular position
                pos.value += pv.linearVelocity * dt;
                rot.value += (glm::quat(pv.angularVelocity * dt) * rot.value * 0.5f);
                rot.value = glm::normalize(rot.value);

                // impose drag
                pv.linearVelocity *= glm::pow(pd.linearDamping, dt);
                pv.angularVelocity *= glm::pow(pd.angularDamping, dt);
            });

        return true;
    }

    void DynamicIntegrateSystem::terminate()
    {
        //
    }
} // namespace blink
