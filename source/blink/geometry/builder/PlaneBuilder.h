/**

    @file      PlaneBuilder.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      4.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/
#pragma once

#include "IGeometryBuilder.h"

namespace blink
{
    class PlaneBuilder : public IGeometryBuilder
    {
    public:
        PlaneBuilder& size(float width, float height);
        PlaneBuilder& segment(uint32 segmentH, uint32 segmentV);
        PlaneBuilder& uvRange(const glm::vec2& uvMin, const glm::vec2& uvMax);
        PlaneBuilder& orient(const glm::quat& orientation);
        PlaneBuilder& translate(const glm::vec3& translation);

        virtual uint32 build(std::vector<glm::vec3>& positionsOut,
                             std::vector<uint16>& indicesOut,
                             std::vector<glm::vec3>* normalsOut = nullptr,
                             std::vector<glm::vec2>* uvsOut = nullptr) override;

    private:
        glm::vec2 m_size{1.0f, 1.0f};
        glm::uvec2 m_segment{1, 1};
        glm::vec2 m_uvMin{0.0f, 0.0f};
        glm::vec2 m_uvMax{1.0f, 1.0f};
        glm::quat m_orientation{1.0f, 0.0f, 0.0f, 0.0f};
        glm::vec3 m_translation{0.0f, 0.0f, 0.0f};
    };
} // namespace blink
