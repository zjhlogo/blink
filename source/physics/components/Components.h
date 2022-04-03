/*********************************************************************
 * \file   Components.h
 * \brief
 *
 * \author zjhlogo
 * \date   2/13 2022
 *********************************************************************/

#pragma once

#include <glm/glm.hpp>

#include <numeric>

namespace blink
{
    struct PhysicsMass
    {
        PhysicsMass();
        PhysicsMass(float m, const glm::mat3& inertiaTensor);
        bool setMass(float m);
        void setInertialTensor(const glm::mat3& inertiaTensor);

        /**
         * Holds the inverse of the mass of the rigid body. It is more useful to hold the inverse mass because integration is simpler,
         * and because in real time simulation it is more useful to have bodies with infinite mass (immovable) then zero mass (completely
         * unstable in numerical simulation).
         */
        float inverseMass;

        /**
         * Holds the inverse of the body's inertia tensor. The inertia tensor provided must not be degenerate (that would mean the body had
         * zero inertia for spinning along one axis). As long as the tensor is finite, it will be invertible. The inverse tensor is use for
         * similar reasons as those for the use of inverse mass.
         *
         * The inertia tensor, unlike the other variables that define a rigid body, is given in body space.
         */
        glm::mat3 inverseInertiaTensor;
    };

    struct PhysicsVelocity
    {
        /**
         * Holds the linear velocity of the rigid body in world space.
         */
        glm::vec3 linearVelocity;

        /**
         * Holds the angular velocity of the rigid body in world space.
         */
        glm::vec3 angularVelocity;
    };

    struct PhysicsDamping
    {
        /**
         * Holds the amount of damping applied to linear motion. Damping is required to remove energy added through numerical instability in the
         * integrator.
         */
        float linearDamping;

        /**
         * Holds the amount of damping applied to angular motion. Damping is required to remove energy added through numerical instability in the
         * integrator.
         */
        float angularDamping;
    };

    struct PhysicsAccumulate
    {
        /**
         * Holds the accumulated force to be applied at the next integration step. this component value will be reset at the end of every frame
         */
        glm::vec3 forceAccum;

        /**
         * Holds the accumulated torque to be applied at the next integration step. this component value will be reset at the end of every frame
         */
        glm::vec3 torqueAccum;
    };

} // namespace blink
