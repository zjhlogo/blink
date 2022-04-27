#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "builtin_uniforms.inc"

layout(location = 0) in vec3 inPosition;

void main()
{
    gl_Position = eu.matLocalToProjection * vec4(inPosition, 1.0);
}