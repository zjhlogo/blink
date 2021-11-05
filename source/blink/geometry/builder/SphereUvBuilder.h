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
        SphereUvBuilder& ringAndSection(uint16 rings, uint16 sections);

        virtual void build(std::vector<glm::vec3>& positionsOut,
                           std::vector<uint16>& indicesOut,
                           std::vector<glm::vec3>* normalsOut = nullptr,
                           std::vector<glm::vec2>* uvsOut = nullptr) override;

    private:
        glm::vec3 m_center{};
        float m_radius{0.5f};
        glm::u16vec2 m_segments{3, 3};
    };
} // namespace blink
