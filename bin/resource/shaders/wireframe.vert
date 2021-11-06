#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform PerFrameUniforms
{
    mat4 matWorldToCamera;
    mat4 matCameraToProjection;
    mat4 matWorldToProjection;
    mat3 matWorldToCameraInvT;
    vec3 cameraPos;
} pfu;

layout(set = 0, binding = 1) uniform PerInstanceUniforms
{
    mat4 matLocalToWorld;
    mat3 matLocalToWorldInvT;
} piu;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

void main()
{
    gl_Position = pfu.matWorldToProjection * piu.matLocalToWorld * vec4(inPosition + inNormal * 0.001, 1.0);
}
