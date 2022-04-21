/**

    @file      PlaneBuilder.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      4.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/

#include "PlaneBuilder.h"

#include <core/modules/IResModule.h>
#include <foundation/BuiltinFormatter.h>
#include <glm/gtx/quaternion.hpp>

namespace blink
{
    PlaneBuilder& PlaneBuilder::size(float width, float height)
    {
        m_size.x = width;
        m_size.y = height;
        return *this;
    }

    PlaneBuilder& PlaneBuilder::segment(uint16 segmentH, uint16 segmentV)
    {
        m_segment.x = segmentH < 1 ? 1 : segmentH;
        m_segment.y = segmentV < 1 ? 1 : segmentV;
        return *this;
    }

    PlaneBuilder& PlaneBuilder::uvRange(const glm::vec2& uvMin, const glm::vec2& uvMax)
    {
        m_uvMin = uvMin;
        m_uvMax = uvMax;
        return *this;
    }

    PlaneBuilder& PlaneBuilder::orient(const glm::quat& orientation)
    {
        m_orientation = orientation;
        return *this;
    }

    PlaneBuilder& PlaneBuilder::translate(const glm::vec3& translation)
    {
        m_translation = translation;
        return *this;
    }

    tstring PlaneBuilder::getUniqueId() const
    {
        //
        return fmt::format("plane_{0}_{1}_{2}_{3}_{4}_{5}", m_size, m_segment, m_uvMin, m_uvMax, m_orientation, m_translation);
    }

    IGeometry* PlaneBuilder::build(bool buildNormal, bool buildUv, glm::mat3* inertiaTensorOut) const
    {
        std::vector<glm::vec3> vertsPos;
        std::vector<uint16> indices;
        std::vector<glm::vec3> vertsNormal;
        std::vector<glm::vec2> vertsUv0;

        std::vector<glm::vec3>* pVertsNormal = nullptr;
        std::vector<glm::vec2>* pVertsUv0 = nullptr;
        if (buildNormal) pVertsNormal = &vertsNormal;
        if (buildUv) pVertsUv0 = &vertsUv0;

        if (!generateData(vertsPos, indices, pVertsNormal, pVertsUv0)) return nullptr;

        auto geometry = getResModule()->createGeometry(getUniqueId(), PrimitiveTopology::TriangleList);
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

    bool PlaneBuilder::generateData(std::vector<glm::vec3>& positionsOut,
                                    std::vector<uint16>& indicesOut,
                                    std::vector<glm::vec3>* normalsOut,
                                    std::vector<glm::vec2>* uvsOut) const
    {
        glm::vec3 normal = glm::rotate(m_orientation, glm::vec3(0.0f, 1.0f, 0.0f));

        glm::vec2 delta(1.0f / m_segment.x, 1.0f / m_segment.y);
        glm::vec2 uvRange = m_uvMax - m_uvMin;

        glm::vec3 currPos(0.0f, 0.0f, 0.0f);
        glm::vec2 currUv = m_uvMin;

        uint16 startIndex = static_cast<uint16>(positionsOut.size());

        for (uint16 y = 0; y < (m_segment.y + 1); ++y)
        {
            currPos.z = -(y * delta.y - 0.5f) * m_size.y;
            currUv.y = y * delta.y * uvRange.y + m_uvMin.y;

            for (uint16 x = 0; x < (m_segment.x + 1); ++x)
            {
                currPos.x = (x * delta.x - 0.5f) * m_size.x;
                currUv.x = x * delta.x * uvRange.x + m_uvMin.x;

                positionsOut.push_back(glm::rotate(m_orientation, currPos) + m_translation);
                if (normalsOut) normalsOut->push_back(normal);
                if (uvsOut) uvsOut->push_back(currUv);
            }
        }

        // build face list
        for (uint16 y = 0; y < m_segment.y; ++y)
        {
            for (uint16 x = 0; x < m_segment.x; ++x)
            {
                uint16 i0 = y * (m_segment.x + 1) + x;
                uint16 i1 = i0 + 1;
                uint16 i2 = (y + 1) * (m_segment.x + 1) + x;
                uint16 i3 = i2 + 1;

                indicesOut.push_back(startIndex + i0);
                indicesOut.push_back(startIndex + i1);
                indicesOut.push_back(startIndex + i2);

                indicesOut.push_back(startIndex + i1);
                indicesOut.push_back(startIndex + i3);
                indicesOut.push_back(startIndex + i2);
            }
        }

        return true;
    }
} // namespace blink
