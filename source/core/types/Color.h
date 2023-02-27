/*********************************************************************
 * \file   Color.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/18/2022
 *********************************************************************/
#pragma once
#include <foundation/BaseTypesGlm.h>

namespace blink
{
    union Color
    {
        uint32_t value{};

        struct
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        };

        explicit Color(uint32_t c)
        {
            // 
            value = c;
        }

        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }

        inline glm::vec4 operator()(const Color& color) const
        {
            //
            return glm::vec4(r, g, b, a) * 0.00392f;
        }

        inline Color operator()(const glm::vec4& color)
        {
            r = static_cast<uint8_t>(color.x * 255);
            g = static_cast<uint8_t>(color.y * 255);
            b = static_cast<uint8_t>(color.z * 255);
            a = static_cast<uint8_t>(color.w * 255);
            return *this;
        }

        static const Color BLACK;
        static const Color WHITE;
        static const Color RED;
        static const Color GREEN;
        static const Color BLUE;
    };
} // namespace blink
