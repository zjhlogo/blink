
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
    bool PhysicsMass::setMass(float m)
    {
        if (m < 0.0f) return false;

        m_mass = m;

        if (m_mass == 0.0f)
        {
            m_inverseMass = std::numeric_limits<float>().max();
        }
        else
        {
            m_inverseMass = 1.0f / m;
        }

        return true;
    }
} // namespace blink
