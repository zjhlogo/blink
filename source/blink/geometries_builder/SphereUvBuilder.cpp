/**

    @file      SphereUvBuilder.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      5.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#include "SphereUvBuilder.h"

#include <core/modules/IResourceModule.h>
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

    SphereUvBuilder& SphereUvBuilder::ringAndSection(uint16_t rings, uint16_t sections)
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

    IGeometry* SphereUvBuilder::build(bool buildNormal, bool buildUv, glm::mat3* inertiaTensorOut) const
    {
        std::vector<glm::vec3> vertsPos;
        std::vector<uint16_t> indices;
        std::vector<glm::vec3> vertsNormal;
        std::vector<glm::vec2> vertsUv0;

        std::vector<glm::vec3>* pVertsNormal = nullptr;
        std::vector<glm::vec2>* pVertsUv0 = nullptr;
        if (buildNormal)
        {
            pVertsNormal = &vertsNormal;
        }
        if (buildUv)
        {
            pVertsUv0 = &vertsUv0;
        }

        float const R = 1.0f / (m_rings - 1);
        float const S = 1.0f / m_sections;

        // calculate the vertex position
        uint16_t startIndex = static_cast<uint16_t>(vertsPos.size());
        for (uint16_t r = 0; r < m_rings; ++r)
        {
            float const y = sin(-glm::half_pi<float>() + glm::pi<float>() * r * R);
            for (uint16_t s = 0; s <= m_sections; ++s)
            {
                float const x = sin(glm::two_pi<float>() * s * S) * sin(glm::pi<float>() * r * R);
                float const z = cos(glm::two_pi<float>() * s * S) * sin(glm::pi<float>() * r * R);

                vertsPos.push_back({x * m_radius + m_center.x, y * m_radius + m_center.y, z * m_radius + m_center.z});
                if (pVertsNormal)
                {
                    pVertsNormal->push_back({x, y, z});
                }
                if (pVertsUv0)
                {
                    pVertsUv0->push_back({s * S, r * R});
                }
            }
        }

        // setup face index
        for (uint16_t r = 0; r < m_rings; ++r)
        {
            int prevRingIndex = r * (m_rings + 1);
            int currRingIndex = prevRingIndex + m_sections + 1;

            for (uint16_t s = 0; s < m_sections; ++s)
            {
                indices.push_back(currRingIndex + s);
                indices.push_back(prevRingIndex + s);
                indices.push_back(currRingIndex + s + 1);

                indices.push_back(currRingIndex + s + 1);
                indices.push_back(prevRingIndex + s);
                indices.push_back(prevRingIndex + s + 1);
            }
        }

        auto geometry = getResourceModule()->createGeometry(getUniqueId(), PrimitiveTopology::TriangleList);
        if (!geometry->uploadData(indices, vertsPos, vertsNormal, vertsUv0))
        {
            SAFE_RELEASE(geometry);
            return nullptr;
        }

        if (inertiaTensorOut)
        {
            *inertiaTensorOut = CalculateInertiaTensor(vertsPos.data(), vertsPos.size());
        }

        return geometry;
    }
} // namespace blink
