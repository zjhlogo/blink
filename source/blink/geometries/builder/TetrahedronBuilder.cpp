/**

    @file      TetrahedronBuilder.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      5.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#include "TetrahedronBuilder.h"

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

    tstring TetrahedronBuilder::getUniqueId() const { return fmt::format("tetrahedron_{0}_{1}", m_center, m_radius); }

    bool TetrahedronBuilder::build(std::vector<glm::vec3>& positionsOut,
                                   std::vector<uint16>& indicesOut,
                                   std::vector<glm::vec3>* normalsOut,
                                   std::vector<glm::vec2>* uvsOut) const
    {
        // build vertex
        glm::vec3 p0(0.0f, m_radius, 0.0f);
        glm::vec3 p2(0.0f, -0.33333f * m_radius, -sqrtf(0.88888f * m_radius * m_radius));
        glm::vec3 p1 = glm::rotate(glm::angleAxis(glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f)), p2);
        glm::vec3 p3 = glm::rotate(glm::angleAxis(glm::radians(-120.0f), glm::vec3(0.0f, 1.0f, 0.0f)), p2);

        uint16 startIndex = static_cast<uint16>(positionsOut.size());
        positionsOut.push_back(p0 + m_center);
        positionsOut.push_back(p1 + m_center);
        positionsOut.push_back(p2 + m_center);
        positionsOut.push_back(p3 + m_center);

        if (normalsOut)
        {
            normalsOut->push_back(glm::normalize(p0));
            normalsOut->push_back(glm::normalize(p1));
            normalsOut->push_back(glm::normalize(p2));
            normalsOut->push_back(glm::normalize(p3));
        }

        if (uvsOut)
        {
            uvsOut->push_back(glm::vec2(0.5f, 1.0f));
            uvsOut->push_back(glm::vec2(0.0f, 0.0f));
            uvsOut->push_back(glm::vec2(0.5f, 0.0f));
            uvsOut->push_back(glm::vec2(1.0f, 0.0f));
        }

        // build index
        indicesOut.push_back(startIndex);
        indicesOut.push_back(startIndex + 1);
        indicesOut.push_back(startIndex + 3);

        indicesOut.push_back(startIndex);
        indicesOut.push_back(startIndex + 2);
        indicesOut.push_back(startIndex + 1);

        indicesOut.push_back(startIndex);
        indicesOut.push_back(startIndex + 3);
        indicesOut.push_back(startIndex + 2);

        indicesOut.push_back(startIndex + 1);
        indicesOut.push_back(startIndex + 2);
        indicesOut.push_back(startIndex + 3);

        return true;
    }
} // namespace blink
