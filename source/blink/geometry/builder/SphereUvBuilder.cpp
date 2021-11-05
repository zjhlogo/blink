/**

    @file      SphereUvBuilder.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      5.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#include "SphereUvBuilder.h"

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
        m_segments.x = rings < 3 ? 3 : rings;
        m_segments.y = sections < 3 ? 3 : sections;
        return *this;
    }

    bool SphereUvBuilder::build(std::vector<glm::vec3>& positionsOut,
                                std::vector<uint16>& indicesOut,
                                std::vector<glm::vec3>* normalsOut,
                                std::vector<glm::vec2>* uvsOut)
    {
        float const R = 1.0f / (m_segments.x - 1);
        float const S = 1.0f / m_segments.y;

        // calculate the vertex position
        uint16 startIndex = static_cast<uint16>(positionsOut.size());
        for (uint16 r = 0; r < m_segments.x; ++r)
        {
            float const y = sin(-glm::half_pi<float>() + glm::pi<float>() * r * R);
            for (uint16 s = 0; s < m_segments.y + 1; ++s)
            {
                float const x = sin(glm::two_pi<float>() * s * S) * sin(glm::pi<float>() * r * R);
                float const z = cos(glm::two_pi<float>() * s * S) * sin(glm::pi<float>() * r * R);

                positionsOut.push_back({x * m_radius + m_center.x, y * m_radius + m_center.y, z * m_radius + m_center.z});
                if (normalsOut) normalsOut->push_back({x, y, z});
                if (uvsOut) uvsOut->push_back({s * S, r * R});
            }
        }

        // setup face index
        for (uint16 r = 0; r < m_segments.x; ++r)
        {
            int prevRingIndex = r * (m_segments.y + 1);
            int currRingIndex = prevRingIndex + m_segments.y + 1;

            for (uint16 s = 0; s < m_segments.y; ++s)
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
