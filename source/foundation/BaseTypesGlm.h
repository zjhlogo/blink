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

namespace blink
{
    glm::vec3 randomVec3Normalized();

} // namespace blink

namespace glm
{
    GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec3 forward()
    {
        return vec3(0.0f, 0.0f, -1.0f);
    }

    GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec3 up()
    {
        return vec3(0.0f, 1.0f, 0.0f);
    }

    GLM_FUNC_QUALIFIER GLM_CONSTEXPR vec3 right()
    {
        return vec3(1.0f, 0.0f, 0.0f);
    }
}
