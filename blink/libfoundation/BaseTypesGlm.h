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
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

NS_BEGIN

extern const glm::ivec2 IVEC2_ZERO;
extern const glm::ivec3 IVEC3_ZERO;
extern const glm::ivec4 IVEC4_ZERO;

extern const glm::vec2  VEC2_ZERO;
extern const glm::vec3  VEC3_ZERO;
extern const glm::vec4  VEC4_ZERO;

extern const glm::vec2  VEC2_HALF;
extern const glm::vec3  VEC3_HALF;
extern const glm::vec4  VEC4_HALF;

extern const glm::vec2  VEC2_ONE;
extern const glm::vec3  VEC3_ONE;
extern const glm::vec4  VEC4_ONE;

extern const glm::vec3  VEC3_PX;
extern const glm::vec3  VEC3_NX;
extern const glm::vec3  VEC3_PY;
extern const glm::vec3  VEC3_NY;
extern const glm::vec3  VEC3_PZ;
extern const glm::vec3  VEC3_NZ;

extern const glm::quat  QUAT_ZERO;

extern const glm::mat3  MAT3_IDENTITY;
extern const glm::mat4  MAT4_IDENTITY;

NS_END
