#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "builtin_uniforms.inc"

layout(set = 0, binding = 3) uniform MaterialUniforms
{
    mat3 matViewToWorld;
    vec3 eyePos;
    float fov;

    vec3 baseColor;
    float twist;
    
    vec3 lightPos;
    
    vec3 lightColor;
    float lightIntensity;
} mu;

layout(location = 0) out vec4 outColor;

const int MAX_STEPS = 100;
const float MAX_DEPTH = 100.0;
const float EPSILON = 0.001;

float sdSphere(vec3 p, float r)
{
    return length(p) - r;
}

float sdBox(vec3 p, vec3 b)
{
    vec3 q = abs(p) - b;
    return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float sdRoundBox(vec3 p, vec3 b, float r)
{
    vec3 q = abs(p) - b;
    return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0) - r;
}

float sdTorus(vec3 p, vec2 t)
{
    vec2 q = vec2(length(p.xy) - t.x, p.z);
    return length(q) - t.y;
}

float opIntersect(float d1, float d2)
{
    return max(d1, d2);
}
float opSmoothIntersect(float d1, float d2, float k)
{
    float h = clamp(0.5 - 0.5*(d2-d1)/k, 0.0, 1.0);
    return mix(d2, d1, h) + k*h*(1.0-h);
}

float opUnion(float distA, float distB)
{
    return min(distA, distB);
}
float opSmoothUnion(float d1, float d2, float k)
{
    float h = clamp(0.5 + 0.5*(d2-d1)/k, 0.0, 1.0);
    return mix(d2, d1, h) - k*h*(1.0-h);
}

float opSubtraction(float d1, float d2)
{
    return max(-d1, d2);
}
float opSmoothSubtraction(float d1, float d2, float k)
{
    float h = clamp(0.5 - 0.5*(d2+d1)/k, 0.0, 1.0);
    return mix(d2, -d1, h) + k*h*(1.0-h);
}

vec3 opTwist(vec3 p, float k)
{
    float c = cos(k*p.y);
    float s = sin(k*p.y);
    mat2  m = mat2(c,-s,s,c);
    return vec3(m*p.xz,p.y);
}

float sdScene(vec3 p)
{
    p = opTwist(p, mu.twist);
    float sphereDist = sdSphere(p - vec3(0, 0, 0), 1.2);
    // return sdTorus(p - vec3(0, 0, 0), vec2(1, 0.5));
    float boxDist = sdBox(p - vec3(0, 0, 0), vec3(1, 1, 1));
    // return sdRoundBox(p - vec3(0, 0, 0), vec3(1, 1, 1), 0.02);
    return opSmoothIntersect(boxDist, sphereDist, 0.01);
}

vec3 getNormal(vec3 p)
{
    return normalize(vec3(
      sdScene(vec3(p.x + EPSILON, p.y, p.z)) - sdScene(vec3(p.x - EPSILON, p.y, p.z)),
      sdScene(vec3(p.x, p.y + EPSILON, p.z)) - sdScene(vec3(p.x, p.y - EPSILON, p.z)),
      sdScene(vec3(p.x, p.y, p.z  + EPSILON)) - sdScene(vec3(p.x, p.y, p.z - EPSILON))
    ));
}

float rayMatching(vec3 rayStart, vec3 rayDir)
{
    float depth = 0;

    for (int i = 0; i < MAX_STEPS; ++i)
    {
        vec3 p = rayStart + rayDir * depth;
        float dist = sdScene(p);

        depth += dist;
        if (depth > MAX_DEPTH || dist < EPSILON) break;
    }

    return depth;
}

vec3 calcRayDir(float fov)
{
    vec2 xy = gl_FragCoord.xy - fu.screenResolution * 0.5;
    float z = fu.screenResolution.y / tan(fov * 0.5);
    return normalize(vec3(xy.x, -xy.y, -z));
}

vec3 blinnPhong(vec3 baseColor, vec3 lightColor, float specularIntensity, vec3 viewDir, vec3 lightDir, vec3 normalDir)
{
    vec3 halfDir = normalize(lightDir + normalDir);
    float dotNL = clamp(dot(normalDir, lightDir), 0.0, 1.0);
    float dotNH = clamp(dot(normalDir, halfDir), 0.0, 1.0);

    vec3 diffuse = baseColor * dotNL;
    vec3 specular = lightColor * pow(dotNH, specularIntensity);
    return diffuse + specular;
}

void main()
{
    vec3 viewDir = mu.matViewToWorld * calcRayDir(mu.fov);
    float depth = rayMatching(mu.eyePos, viewDir);
    if (depth > MAX_DEPTH - EPSILON)
    {
        outColor = vec4(0, 0, 0, 1);
        return;
    }

    vec3 hitPoint = mu.eyePos + viewDir * depth;
    vec3 lightDir = normalize(mu.lightPos - hitPoint);
    vec3 normalDir = getNormal(hitPoint);

    vec3 color = blinnPhong(mu.baseColor, mu.lightColor, mu.lightIntensity, -viewDir, lightDir, normalDir);
    outColor = vec4(color, 1);
}
