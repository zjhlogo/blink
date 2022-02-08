#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "uniforms.inc"

layout(set = 0, binding = 3) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragWorldPos;

layout(location = 0) out vec4 outColor;

void main()
{
    vec3 viewDir = normalize(pfu.cameraPos - fragWorldPos);
    vec3 lightDir = normalize(pfu.lightPos - fragWorldPos);
    vec3 normalDir = normalize(fragNormal);
    vec3 halfDir = normalize(lightDir + normalDir);

    float dotNV = clamp(dot(normalDir, viewDir), 0.0, 1.0);
    float dotNH = clamp(dot(normalDir, halfDir), 0.0, 1.0);

    vec3 diffuse = texture(texSampler, fragTexCoord).xyz * dotNV;
    vec3 specular = pfu.lightColorAndIntensity.xyz * pow(dotNH, pfu.lightColorAndIntensity.w);

    outColor = vec4(diffuse + specular, 1.0);
}
