
/*********************************************************************
 * \file   UniformTypes.h
 * \brief
 *
 * \author zjhlogo
 * \date   4/25 2022
 *********************************************************************/
#pragma once
#include <foundation/BaseTypesGlm.h>

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

    enum class UniformBinding
    {
        Camera = 0,
        Instance,
        Material,
    };

    template <typename T> inline UniformType getUniformType() { return UniformType::None; }
    template <> inline UniformType getUniformType<int>() { return UniformType::Int; }
    template <> inline UniformType getUniformType<float>() { return UniformType::Float; }
    template <> inline UniformType getUniformType<glm::vec2>() { return UniformType::Vec2; }
    template <> inline UniformType getUniformType<glm::vec3>() { return UniformType::Vec3; }
    template <> inline UniformType getUniformType<glm::vec4>() { return UniformType::Vec4; }
    template <> inline UniformType getUniformType<glm::mat3>() { return UniformType::Mat3; }
    template <> inline UniformType getUniformType<glm::mat4>() { return UniformType::Mat4; }

} // namespace blink
