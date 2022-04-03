/**

    @file      SphereUvBuilder.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      5.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#include "SphereUvBuilder.h"

#include <foundation/BuiltinFormatter.h>
#include <glm/gtx/quaternion.hpp>

namespace blink
{
    SphereUvBuilder& SphereUvBuilder::center(float x, float y, float z)
    {
        m_center.x = x;
        m_center.y = y;
        m_center.z = z;
        return *this;
    }

    SphereUvBuilder& SphereUvBuilder::radius(float radius)
    {
        m_radius = radius;
        return *this;
    }

    SphereUvBuilder& SphereUvBuilder::ringAndSection(uint16 rings, uint16 sections)
    {
        m_rings = rings < 3 ? 3 : rings;
        m_sections = sections < 3 ? 3 : sections;
        return *this;
    }

    tstring SphereUvBuilder::getUniqueId() const
    {
        //
        return fmt::format("sphereuv_{0}_{1}_{2}_{3}", m_center, m_radius, m_rings, m_sections);
    }

    bool SphereUvBuilder::build(std::vector<glm::vec3>& positionsOut,
                                std::vector<uint16>& indicesOut,
                                std::vector<glm::vec3>* normalsOut,
                                std::vector<glm::vec2>* uvsOut) const
    {
        float const R = 1.0f / (m_rings - 1);
        float const S = 1.0f / m_sections;

        // calculate the vertex position
        uint16 startIndex = static_cast<uint16>(positionsOut.size());
        for (uint16 r = 0; r < m_rings; ++r)
        {
            float const y = sin(-glm::half_pi<float>() + glm::pi<float>() * r * R);
            for (uint16 s = 0; s <= m_sections; ++s)
            {
                float const x = sin(glm::two_pi<float>() * s * S) * sin(glm::pi<float>() * r * R);
                float const z = cos(glm::two_pi<float>() * s * S) * sin(glm::pi<float>() * r * R);

                positionsOut.push_back({x * m_radius + m_center.x, y * m_radius + m_center.y, z * m_radius + m_center.z});
                if (normalsOut) normalsOut->push_back({x, y, z});
                if (uvsOut) uvsOut->push_back({s * S, r * R});
            }
        }

        // setup face index
        for (uint16 r = 0; r < m_rings; ++r)
        {
            int prevRingIndex = r * (m_rings + 1);
            int currRingIndex = prevRingIndex + m_sections + 1;

            for (uint16 s = 0; s < m_sections; ++s)
            {
                indicesOut.push_back(currRingIndex + s);
                indicesOut.push_back(prevRingIndex + s);
                indicesOut.push_back(currRingIndex + s + 1);

                indicesOut.push_back(currRingIndex + s + 1);
                indicesOut.push_back(prevRingIndex + s);
                indicesOut.push_back(prevRingIndex + s + 1);
            }
        }

        return true;
    }
} // namespace blink
