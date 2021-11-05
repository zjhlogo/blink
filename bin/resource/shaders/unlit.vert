#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform PerFrameUniforms
{
    mat4 matWorldToCamera;
    mat4 matCameraToProjection;
    mat4 matWorldToProjection;
} pfu;

layout(binding = 1) uniform PerInstanceUniforms
{
    mat4 matLocalToWorld;
} piu;

layout(location = 0) in vec3 inPosition;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec2 fragTexCoord;

void main()
{
    gl_Position = pfu.matWorldToProjection * piu.matLocalToWorld * vec4(inPosition, 1.0);
    fragTexCoord = vec2(inTexCoord.x, 1.0 - inTexCoord.y); // flip y for vulkan
}
