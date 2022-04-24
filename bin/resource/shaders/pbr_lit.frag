#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "uniforms.inc"

layout(set = 0, binding = 2) uniform PerMaterialUniforms
{
    float roughness;
    float metallic;
    vec3 color;
} pmu;

const float PI = 3.14159265359;

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragWorldPos;

layout(location = 0) out vec4 outColor;

vec3 materialColor()
{
    return pmu.color;
}

float D_GGX(float dotNH, float roughness)
{
    //float alpha = roughness * roughness;
    float alpha2 = roughness * roughness;
    float denom = dotNH * dotNH * (alpha2 - 1.0) + 1.0;
    return (alpha2) / (PI * denom * denom);
}

float G_SchlicksmithGGX(float dotNL, float dotNV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    float GL = dotNL / (dotNL * (1.0 - k) + k);
    float GV = dotNV / (dotNV * (1.0 - k) + k);
    return GL * GV;
}

vec3 F_Schlick(float cosTheta, float metallic)
{
    vec3 F0 = mix(vec3(0.04), materialColor(), metallic);
    vec3 F = F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
    return F;
}

vec3 BRDF(vec3 L, vec3 V, vec3 N, float metallic, float roughness)
{
    vec3 H = normalize(V + L);
    float dotNV = clamp(dot(N, V), 0.001, 1.0);
    float dotNL = clamp(dot(N, L), 0.001, 1.0);
    float dotLH = clamp(dot(L, H), 0.001, 1.0);
    float dotNH = clamp(dot(N, H), 0.001, 1.0);

    vec3 lightColor = pfu.lightColorAndIntensity.xyz;
    vec3 color = vec3(0.0);

    float D = D_GGX(dotNH, roughness);
    float G = G_SchlicksmithGGX(dotNL, dotNV, roughness);
    vec3 F = F_Schlick(dotNV, metallic);
    vec3 spec = D * F * G / (4.0 * dotNL * dotNV);
    color += spec * dotNL * lightColor;

    return color;
}

void main()
{
    vec3 N = normalize(fragNormal);
    vec3 V = normalize(pfu.cameraPos - fragWorldPos);
    vec3 L = normalize(pfu.lightPos - fragWorldPos);

    vec3 Lo = vec3(0.0);
    Lo += BRDF(L, V, N, pmu.metallic, pmu.roughness);

    vec3 color = materialColor() * 0.02;
    color += Lo;

    color = pow(color, vec3(0.4545));

    outColor = vec4(color, 1.0);
}
