#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "builtin_uniforms.hlsl"

layout(location = 0) in vec3 inPosition;
layout(location = 10) in vec4 inColor;

layout(location = 0) out vec4 fragColor;

void main()
{
    gl_Position = eu.matLocalToProjection * vec4(inPosition, 1.0);
    fragColor = inColor;
}
