/*!
 * \file BaseTypesGlm.h
 *
 * \author zjhlogo
 * \date 2019/07/26
 *
 *
 */
#pragma once
#include "BaseTypes.h"

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <random>

namespace blink
{
    glm::vec3 randomVec3Normalized();
    glm::vec3 randomVec3Normalized(std::default_random_engine& re);
    float randomRange(std::default_random_engine& re, float min, float max);
    float randomRadian(std::default_random_engine& re);
    glm::quat randomQuat(std::default_random_engine& re);
} // namespace blink

namespace glm
{
    GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec3 forward()
    {
        return {0.0f, 0.0f, -1.0f};
    }

    GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec3 up()
    {
        return {0.0f, 1.0f, 0.0f};
    }

    GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec3 right()
    {
        return {1.0f, 0.0f, 0.0f};
    }

    GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec3 rotate(const quat& q, const vec3& v)
    {
        glm::vec3 q_xyz(q.x, q.y, q.z);
        return v + 2.0f * cross(q_xyz, cross(q_xyz, v) + q.w * v);
    }
} // namespace glm
