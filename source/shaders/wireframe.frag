#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "builtin_uniforms.hlsl"

layout(set = 0, binding = 2) uniform MaterialUniforms
{
    vec3 color;
} mu;

layout(location = 0) out vec4 outColor;

void main()
{
    outColor = vec4(mu.color, 1.0);
}
