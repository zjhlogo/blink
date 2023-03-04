#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "builtin_uniforms.hlsl"

layout(set = 0, binding = 3) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragLocalPos;

layout(location = 0) out vec4 outColor;

vec2 sampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= vec2(0.1591, 0.3183);
    uv += 0.5;
    return uv;
}

void main()
{
    vec2 uv = sampleSphericalMap(normalize(fragLocalPos));
    vec3 color = textureLod(texSampler, uv, 0).rgb;
    
    outColor = vec4(color, 1.0);
}
