#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "uniforms.inc"

layout(set = 0, binding = 2) uniform PerMaterialUniforms
{
    vec3 color;
} pmu;

layout(location = 0) out vec4 outColor;

void main()
{
    outColor = vec4(pmu.color, 1.0);
}
