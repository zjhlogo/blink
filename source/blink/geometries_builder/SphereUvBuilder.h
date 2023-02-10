/**

    @file      SphereUvBuilder.h
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
    class SphereUvBuilder : public IGeometryBuilder
    {
    public:
        SphereUvBuilder& center(float x, float y, float z);
        SphereUvBuilder& radius(float radius);
        SphereUvBuilder& ringAndSection(uint16_t rings, uint16_t sections);

        virtual tstring getUniqueId() const override;
        IGeometry* build(bool buildNormal = true, bool buildUv = true, glm::mat3* inertiaTensorOut = nullptr) const;

    private:
        glm::vec3 m_center{};
        float m_radius{0.5f};
        uint16_t m_rings{15};
        uint16_t m_sections{15};
    };
} // namespace blink
