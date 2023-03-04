#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "builtin_uniforms.hlsl"

layout(location = 0) in vec3 inPosition;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec2 fragTexCoord;

void main()
{
    gl_Position = cu.matWorldToProjection * pushData.matLocalToWorld * vec4(inPosition, 1.0);
    fragTexCoord = inTexCoord;
}
