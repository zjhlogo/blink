/**

    @file      TetrahedronBuilder.h
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
    class TetrahedronBuilder : public IGeometryBuilder
    {
    public:
        TetrahedronBuilder& center(float x, float y, float z);
        TetrahedronBuilder& radius(float radius);

        virtual tstring getUniqueId() const override;
        IGeometry* build(bool buildNormal = true, bool buildUv = true, glm::mat3* inertiaTensorOut = nullptr) const;

    private:
        glm::vec3 m_center{};
        float m_radius{0.5f};
    };
} // namespace blink
