#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "uniforms.inc"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

void main()
{
    gl_Position = pfu.matWorldToProjection * piu.matLocalToWorld * vec4(inPosition + inNormal * 0.001, 1.0);
}
