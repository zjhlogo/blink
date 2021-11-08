/**

    @file      BoxBuilder.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      5.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

#include "IGeometryBuilder.h"

namespace blink
{
    class BoxBuilder : public IGeometryBuilder
    {
    public:
        BoxBuilder& size(float width, float height, float depth);
        BoxBuilder& segment(uint16 widthSegment, uint16 heightSegment, uint16 depthSegment);

        virtual tstring getUniqueId() const override;

        virtual bool build(std::vector<glm::vec3>& positionsOut,
                           std::vector<uint16>& indicesOut,
                           std::vector<glm::vec3>* normalsOut = nullptr,
                           std::vector<glm::vec2>* uvsOut = nullptr) const override;

    private:
        glm::vec3 m_size{1.0f, 1.0f, 1.0f};
        glm::u16vec3 m_segment{1, 1, 1};
    };
} // namespace blink
