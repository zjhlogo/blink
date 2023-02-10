/*********************************************************************
 * \file   Components.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   2/13 2022
 *********************************************************************/
#include "Components.h"

#include <foundation/BaseTypesGlm.h>

namespace blink
{
    PhysicsMass::PhysicsMass()
    {
        setMass(1.0f);
        setInertialTensor(glm::identity<glm::mat3>());
    }

    PhysicsMass::PhysicsMass(float m, const glm::mat3& inertiaTensor)
    {
        setMass(m);
        setInertialTensor(inertiaTensor);
    }

    bool PhysicsMass::setMass(float m)
    {
        if (m < 0.0f) return false;

        if (m == 0.0f) { inverseMass = std::numeric_limits<float>::max(); }
        else { inverseMass = 1.0f / m; }

        return true;
    }

    void PhysicsMass::setInertialTensor(const glm::mat3& inertiaTensor)
    {
        //
        inverseInertiaTensor = glm::inverse(inertiaTensor);
    }
} // namespace blink
