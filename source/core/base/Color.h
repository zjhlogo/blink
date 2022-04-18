
/*********************************************************************
 * \file   Color.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/18/2022
 *********************************************************************/
#pragma once
#include <foundation/BaseTypes.h>
#include <foundation/BaseTypesGlm.h>

namespace blink
{
    union Color
    {
        uint32 value;
        struct
        {
            uint8 b;
            uint8 g;
            uint8 r;
            uint8 a;
        };

        Color(uint32 c)
        {
            // 
            value = c;
        }

        Color(uint8 _r, uint8 _g, uint8 _b, uint8 _a)
        {
            r = _r;
            g = _g;
            b = _b;
            a = _a;
        }

        inline glm::vec4 operator()(const Color& color)
        {
            //
            return glm::vec4(r, g, b, a) * 0.00392f;
        }

        inline Color operator()(const glm::vec4& color)
        {
            r = (uint8)(color.x * 255);
            g = (uint8)(color.y * 255);
            b = (uint8)(color.z * 255);
            a = (uint8)(color.w * 255);
        }

        static const Color BLACK;
        static const Color WHITE;
        static const Color RED;
        static const Color GREEN;
        static const Color BLUE;
    };
} // namespace blink
