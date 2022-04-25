#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "builtin_uniforms.inc"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragNormal;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragWorldPos;

void main()
{
    vec4 worldPos = piu.matLocalToWorld * vec4(inPosition, 1.0);
    
    fragWorldPos = worldPos.xyz;
    fragNormal = piu.matLocalToWorldInvT * inNormal;
    fragTexCoord = vec2(inTexCoord.x, 1.0 - inTexCoord.y); // flip y for vulkan

    gl_Position = pcu.matWorldToProjection * worldPos;
}
