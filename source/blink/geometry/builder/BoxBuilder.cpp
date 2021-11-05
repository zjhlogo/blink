/**

    @file      BoxBuilder.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      5.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/

#include "BoxBuilder.h"
#include "PlaneBuilder.h"

namespace blink
{
    BoxBuilder& BoxBuilder::size(float width, float height, float depth)
    {
        m_size.x = width;
        m_size.y = height;
        m_size.z = depth;
        return *this;
    }

    BoxBuilder& BoxBuilder::segment(uint16 widthSegment, uint16 heightSegment, uint16 depthSegment)
    {
        m_segment.x = widthSegment < 1 ? 1 : widthSegment;
        m_segment.y = heightSegment < 1 ? 1 : heightSegment;
        m_segment.z = depthSegment < 1 ? 1 : depthSegment;
        return *this;
    }

    bool BoxBuilder::build(std::vector<glm::vec3>& positionsOut,
                           std::vector<uint16>& indicesOut,
                           std::vector<glm::vec3>* normalsOut,
                           std::vector<glm::vec2>* uvsOut)
    {
        PlaneBuilder builder;

        // top
        builder.size(m_size.x, m_size.z)
            .segment(m_segment.x, m_segment.z)
            .translate(glm::vec3(0.0f, 0.5f * m_size.y, 0.0f))
            .build(positionsOut, indicesOut, normalsOut, uvsOut);

        // bottom
        builder.size(m_size.x, m_size.z)
            .segment(m_segment.x, m_segment.z)
            .orient(glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)))
            .translate(glm::vec3(0.0f, -0.5f * m_size.y, 0.0f))
            .build(positionsOut, indicesOut, normalsOut, uvsOut);

        // front
        builder.size(m_size.x, m_size.y)
            .segment(m_segment.x, m_segment.y)
            .orient(glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)))
            .translate(glm::vec3(0.0f, 0.0f, 0.5f * m_size.z))
            .build(positionsOut, indicesOut, normalsOut, uvsOut);

        // back
        builder.size(m_size.x, m_size.y)
            .segment(m_segment.x, m_segment.y)
            .orient(glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)))
            .translate(glm::vec3(0.0f, 0.0f, -0.5f * m_size.z))
            .build(positionsOut, indicesOut, normalsOut, uvsOut);

        // left
        builder.size(m_size.y, m_size.z)
            .segment(m_segment.y, m_segment.z)
            .orient(glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)))
            .translate(glm::vec3(-0.5f * m_size.x, 0.0f, 0.0f))
            .build(positionsOut, indicesOut, normalsOut, uvsOut);

        // right
        builder.size(m_size.y, m_size.z)
            .segment(m_segment.y, m_segment.z)
            .orient(glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)))
            .translate(glm::vec3(0.5f * m_size.x, 0.0f, 0.0f))
            .build(positionsOut, indicesOut, normalsOut, uvsOut);

        return true;
    }
} // namespace blink
