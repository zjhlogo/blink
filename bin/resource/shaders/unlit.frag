#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform PerFrameUniforms
{
    mat4 matWorldToCamera;
    mat4 matCameraToProjection;
    mat4 matWorldToProjection;
    mat3 matWorldToCameraInvT;
    vec3 cameraPos;
    vec3 cameraDir;
} pfu;

layout(set = 0, binding = 1) uniform PerInstanceUniforms
{
    mat4 matLocalToWorld;
    mat3 matLocalToWorldInvT;
} piu;

layout(set = 0, binding = 2) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main()
{
    float factor = clamp(dot(fragNormal, -pfu.cameraDir), 0, 1);
    outColor = vec4(texture(texSampler, fragTexCoord).xyz * factor, 1.0);
}
