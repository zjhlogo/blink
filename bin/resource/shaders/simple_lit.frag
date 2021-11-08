#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "uniforms.inc"

layout(set = 0, binding = 2) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main()
{
    float factor = clamp(dot(fragNormal, -pfu.cameraDir), 0, 1);
    outColor = vec4(texture(texSampler, fragTexCoord).xyz * factor, 1.0);
}
