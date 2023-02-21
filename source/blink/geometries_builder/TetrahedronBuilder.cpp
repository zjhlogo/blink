/**

    @file      TetrahedronBuilder.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      5.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#include "TetrahedronBuilder.h"

#include <core/modules/IResourceModule.h>
#include <foundation/BuiltinFormatter.h>
#include <glm/gtx/quaternion.hpp>

namespace blink
{
    TetrahedronBuilder& TetrahedronBuilder::center(float x, float y, float z)
    {
        m_center.x = x;
        m_center.y = y;
        m_center.z = z;
        return *this;
    }

    TetrahedronBuilder& TetrahedronBuilder::radius(float radius)
    {
        m_radius = radius;
        return *this;
    }

    tstring TetrahedronBuilder::getUniqueId() const
    {
        //
        return fmt::format("tetrahedron_{0}_{1}", m_center, m_radius);
    }

    IGeometry* TetrahedronBuilder::build(bool buildNormal, bool buildUv, glm::mat3* inertiaTensorOut) const
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

        // build vertex
        glm::vec3 p0(m_radius, 0.0f, -m_radius / sqrtf(2.0f));
        glm::vec3 p2(-m_radius, 0.0f, -m_radius / sqrtf(2.0f));
        glm::vec3 p1(0.0f, -m_radius, m_radius / sqrtf(2.0f));
        glm::vec3 p3(0.0f, m_radius, m_radius / sqrtf(2.0f));

        uint16_t startIndex = static_cast<uint16_t>(vertsPos.size());
        vertsPos.push_back(p0 + m_center);
        vertsPos.push_back(p1 + m_center);
        vertsPos.push_back(p2 + m_center);
        vertsPos.push_back(p3 + m_center);

        if (pVertsNormal)
        {
            pVertsNormal->push_back(glm::normalize(p0));
            pVertsNormal->push_back(glm::normalize(p1));
            pVertsNormal->push_back(glm::normalize(p2));
            pVertsNormal->push_back(glm::normalize(p3));
        }

        if (pVertsUv0)
        {
            pVertsUv0->push_back(glm::vec2(0.5f, 1.0f));
            pVertsUv0->push_back(glm::vec2(0.0f, 0.0f));
            pVertsUv0->push_back(glm::vec2(0.5f, 0.0f));
            pVertsUv0->push_back(glm::vec2(1.0f, 0.0f));
        }

        // build index
        indices.push_back(startIndex);
        indices.push_back(startIndex + 1);
        indices.push_back(startIndex + 3);

        indices.push_back(startIndex);
        indices.push_back(startIndex + 2);
        indices.push_back(startIndex + 1);

        indices.push_back(startIndex);
        indices.push_back(startIndex + 3);
        indices.push_back(startIndex + 2);

        indices.push_back(startIndex + 1);
        indices.push_back(startIndex + 2);
        indices.push_back(startIndex + 3);

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
