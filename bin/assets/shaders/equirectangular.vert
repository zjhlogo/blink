#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "builtin_uniforms.hlsl"

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec3 fragLocalPos;

void main()
{
    fragLocalPos = inPosition;
    gl_Position = cu.matWorldToProjection * pushData.matLocalToWorld * vec4(inPosition, 1.0);
}
