#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "builtin_uniforms.inc"

layout(location = 0) in vec3 inPosition;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec2 fragTexCoord;

void main()
{
    gl_Position = piu.matLocalToProjection * vec4(inPosition, 1.0);
    fragTexCoord = vec2(inTexCoord.x, 1.0 - inTexCoord.y); // flip y for vulkan
}
