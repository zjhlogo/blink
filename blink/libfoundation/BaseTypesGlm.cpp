/*!
 * \file BaseTypesGlm.cpp
 *
 * \author zjhlogo
 * \date 2019/07/26
 *
 * 
 */
#include "BaseTypesGlm.h"

NS_BEGIN

const glm::ivec2 IVEC2_ZERO = glm::ivec2(0, 0);
const glm::ivec3 IVEC3_ZERO = glm::ivec3(0, 0, 0);
const glm::ivec4 IVEC4_ZERO = glm::ivec4(0, 0, 0, 0);

const glm::vec2 VEC2_ZERO = glm::vec2(0.0f, 0.0f);
const glm::vec3 VEC3_ZERO = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec4 VEC4_ZERO = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

const glm::vec2 VEC2_HALF = glm::vec2(0.5f, 0.5f);
const glm::vec3 VEC3_HALF = glm::vec3(0.5f, 0.5f, 0.5f);
const glm::vec4 VEC4_HALF = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);

const glm::vec2 VEC2_ONE = glm::vec2(1.0f, 1.0f);
const glm::vec3 VEC3_ONE = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec4 VEC4_ONE = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

const glm::vec3 VEC3_PX = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 VEC3_NX = glm::vec3(-1.0f, 0.0f, 0.0f);
const glm::vec3 VEC3_PY = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 VEC3_NY = glm::vec3(0.0f, -1.0f, 0.0f);
const glm::vec3 VEC3_PZ = glm::vec3(0.0f, 0.0f, 1.0f);
const glm::vec3 VEC3_NZ = glm::vec3(0.0f, 0.0f, -1.0f);

const glm::quat QUAT_ZERO = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

const glm::mat3 MAT3_IDENTITY = glm::mat3(
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f);

const glm::mat4 MAT4_IDENTITY = glm::mat4(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f);

NS_END
