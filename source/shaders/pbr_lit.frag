#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "builtin_uniforms.hlsl"

layout(set = 0, binding = 2) uniform MaterialUniforms
{
    vec3 color;
    float roughness;
    float metallic;
} mu;

layout(set = 0, binding = 3) uniform sampler2D texAlbedo;
layout(set = 0, binding = 4) uniform sampler2D texNormal;
layout(set = 0, binding = 5) uniform sampler2D texMetallicRoughness;
layout(set = 0, binding = 6) uniform sampler2D texAmbientOcclusion;
layout(set = 0, binding = 7) uniform sampler2D texEmissive;
layout(set = 0, binding = 8) uniform sampler2D texIrradiance;

const float PI = 3.14159;

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragWorldPos;

layout(location = 0) out vec4 outColor;

float D_GGX(float nh, float roughness)
{
    float squareRoughness = roughness * roughness;
    float denom = nh * nh * (squareRoughness - 1.0) + 1.0;
    return (squareRoughness) / (PI * denom * denom);
}

float G_SchlicksmithGGX(float nl, float nv, float roughness)
{
    float k = pow(roughness + 1.0, 2.0) / 8.0;
    float gl = nl / mix(nl, 1.0, k);
    float gr = nv / mix(nv, 1.0, k);
    return gl * gr;
}

vec3 F_UE(vec3 albedo, float vh, float metallic)
{
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    vec3 F = F0 + (1.0 - F0) * exp2((-5.55473 * vh - 6.98316) * vh);
    return F;
}

vec2 sampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= vec2(0.1591, 0.3183);
    uv += 0.5;
    return uv;
}

void main()
{
    vec3 normalDir = normalize(fragNormal);
    vec3 viewDir = normalize(cu.cameraPos - fragWorldPos);
    vec3 lightDir = normalize(fu.lightPos);
    vec3 halfDir = normalize(viewDir + lightDir);

    float nl = clamp(dot(normalDir, lightDir), 0.0001, 1.0);
    float nv = clamp(dot(normalDir, viewDir), 0.0001, 1.0);
    float vh = clamp(dot(viewDir, halfDir), 0.0001, 1.0);
    float lh = clamp(dot(lightDir, halfDir), 0.0001, 1.0);
    float nh = clamp(dot(normalDir, halfDir), 0.0001, 1.0);
    vec3 mr = texture(texMetallicRoughness, fragTexCoord).xyz;

    vec3 albedo = texture(texAlbedo, fragTexCoord).xyz;
    vec3 irradiance = texture(texIrradiance, sampleSphericalMap(normalDir)).rgb;
//    vec3 reflectDir = reflect(-viewDir, normalDir);
//    outColor = textureLod(texIrradiance, sampleSphericalMap(reflectDir), 0.0);

    // normal distribution
    float D = D_GGX(nh, mr.y);

    // geometry
    float G = G_SchlicksmithGGX(nl, nv, mr.y);

    // fresnel
    vec3 F = F_UE(albedo, vh, mr.x);

    // specular
    vec3 specular = ((D * G * F * 0.25) / (nv * nl)) * fu.lightColor * nl;

    vec3 kd = (1 - F) * (1 - mr.x);
    vec3 diffuse = kd * (albedo * irradiance / PI) * fu.lightColor * nl;
    outColor = vec4(diffuse + specular, 1.0);
}
