
/*********************************************************************
 * \file   Components.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   2/13 2022
 *********************************************************************/
#include "Components.h"

namespace blink
{
    PhysicsMass::PhysicsMass(float m, const glm::mat3& inertiaTensor)
    {
        setMass(m);
        setInertialTensor(inertiaTensor);
    }

    bool PhysicsMass::setMass(float m)
    {
        if (m < 0.0f) return false;

        if (m == 0.0f)
        {
            m_inverseMass = std::numeric_limits<float>().max();
        }
        else
        {
            m_inverseMass = 1.0f / m;
        }

        return true;
    }

    void PhysicsMass::setInertialTensor(const glm::mat3& inertiaTensor)
    {
        //
        m_inverseInertiaTensor = glm::inverse(inertiaTensor);
    }
} // namespace blink
