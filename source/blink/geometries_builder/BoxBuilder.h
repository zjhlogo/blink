/*********************************************************************
 * \file   BoxBuilder.h
 * \brief
 *
 * \author zjhlogo
 * \date   February 2023
 *********************************************************************/
#pragma once

#include "IGeometryBuilder.h"

namespace blink
{
    class BoxBuilder : public IGeometryBuilder
    {
    public:
        BoxBuilder& size(float width, float height, float depth);
        BoxBuilder& size(const glm::vec3& sz);
        BoxBuilder& segment(uint16_t widthSegment, uint16_t heightSegment, uint16_t depthSegment);

        tstring getUniqueId() const override;

        IGeometry* build(bool buildNormal = true, bool buildUv = true, glm::mat3* inertiaTensorOut = nullptr) const;

    private:
        glm::vec3 m_size{1.0f, 1.0f, 1.0f};
        glm::u16vec3 m_segment{1, 1, 1};
    };
} // namespace blink
