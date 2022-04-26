
/*********************************************************************
 * \file   UniformTypes.h
 * \brief
 *
 * \author zjhlogo
 * \date   4/25 2022
 *********************************************************************/
#pragma once
#include <foundation/BaseTypes.h>

namespace blink
{
    enum class UniformType
    {
        None = 0,
        Int,
        Float,
        Vec2,
        Vec3,
        Vec4,
        Mat3,
        Mat4,
        NumTypes
    };
} // namespace blink
