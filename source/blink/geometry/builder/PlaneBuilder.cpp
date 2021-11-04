/**

    @file      PlaneBuilder.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      4.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/

#include "PlaneBuilder.h"

#include <glm/gtx/quaternion.hpp>

namespace blink
{
    PlaneBuilder& PlaneBuilder::size(float width, float height)
    {
        m_size.x = width;
        m_size.y = height;
        return *this;
    }

    PlaneBuilder& PlaneBuilder::segment(uint32 segmentH, uint32 segmentV)
    {
        m_segment.x = segmentH;
        m_segment.y = segmentV;
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

    uint32 PlaneBuilder::build(std::vector<glm::vec3>& positionsOut,
                               std::vector<uint16>& indicesOut,
                               std::vector<glm::vec3>* normalsOut,
                               std::vector<glm::vec2>* uvsOut)
    {
        glm::vec3 normal = glm::rotate(m_orientation, glm::vec3(0.0f, 1.0f, 0.0f));

        glm::vec2 delta(1.0f / m_segment.x, 1.0f / m_segment.y);
        glm::vec2 uvRange = m_uvMax - m_uvMin;

        glm::vec3 currPos(0.0f, 0.0f, 0.0f);
        glm::vec2 currUv = m_uvMin;

        uint16 startIndex = static_cast<uint16>(positionsOut.size());

        for (uint32 y = 0; y < (m_segment.y + 1); ++y)
        {
            currPos.z = -(y * delta.y - 0.5f) * m_size.y;
            currUv.y = y * delta.y * uvRange.y + m_uvMin.y;

            for (uint32 x = 0; x < (m_segment.x + 1); ++x)
            {
                currPos.x = (x * delta.x - 0.5f) * m_size.x;
                currUv.x = x * delta.x * uvRange.x + m_uvMin.x;

                positionsOut.push_back(glm::rotate(m_orientation, currPos) + m_translation);
                if (normalsOut) normalsOut->push_back(normal);
                if (uvsOut) uvsOut->push_back(currUv);
            }
        }

        // build face list
        for (uint32 y = 0; y < m_segment.y; ++y)
        {
            for (uint32 x = 0; x < m_segment.x; ++x)
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

        return static_cast<uint32>(positionsOut.size()) - startIndex;
    }
} // namespace blink
