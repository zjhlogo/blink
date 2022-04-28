#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "builtin_uniforms.inc"

layout(set = 0, binding = 3) uniform MaterialUniforms
{
    mat3 matRotate;
    float fov;
} mu;

layout(location = 0) out vec4 outColor;

const int MAX_STEPS = 20;
const float MAX_DEPTH = 100.0;
const float EPSILON = 0.001;

float sdfSphere(vec3 p, float r)
{
    return length(p) - r;
}

float sdfBox(vec3 p, vec3 b)
{
    vec3 q = abs(p) - b;
    return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float sdfTorus(vec3 p, vec2 t)
{
    vec2 q = vec2(length(p.xy) - t.x, p.z);
    return length(q) - t.y;
}

float sdfScene(vec3 p)
{
    float dist = sdfSphere(p - vec3(0, 0, 0), 1.0);
    return dist;
    // float dist2 = sdfTorus(p - vec3(0.3, 0, 0), vec2(0.1, 0.02));
    // float dist3 = sdfBox(p, vec3(0.1, 0.1, 0.1));
    // return min(min(dist, dist2), dist3);
}

vec3 getNormal(vec3 p)
{
    return normalize(vec3(
      sdfScene(vec3(p.x + EPSILON, p.y, p.z)) - sdfScene(vec3(p.x - EPSILON, p.y, p.z)),
      sdfScene(vec3(p.x, p.y + EPSILON, p.z)) - sdfScene(vec3(p.x, p.y - EPSILON, p.z)),
      sdfScene(vec3(p.x, p.y, p.z  + EPSILON)) - sdfScene(vec3(p.x, p.y, p.z - EPSILON))
    ));
}

float rayMatching(vec3 rayStart, vec3 rayDir)
{
    float depth = 0;

    for (int i = 0; i < MAX_STEPS; ++i)
    {
        vec3 p = rayStart + rayDir * depth;
        float dist = sdfScene(p);

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

void main()
{
    vec3 dir = calcRayDir(mu.fov);
    vec3 eye = vec3(0.0, 0.0, 5.0);
    float depth = rayMatching(eye, dir);
    if (depth > MAX_DEPTH - EPSILON)
    {
        outColor = vec4(0, 0, 0, 1);
        return;
    }

    vec3 color = vec3(1, 0, 0);
    vec3 light1Pos = vec3(4.0 * sin(fu.time),
                          2.0,
                          4.0 * cos(fu.time));
    vec3 light1Intensity = vec3(0.4, 0.4, 0.4);

    vec3 hitPoint = eye + dir * depth;

    vec3 viewDir = -dir;
    vec3 lightDir = normalize(light1Pos - hitPoint);
    vec3 normalDir = getNormal(hitPoint);
    vec3 halfDir = normalize(lightDir + normalDir);

    float dotNL = clamp(dot(normalDir, lightDir), 0.0, 1.0);
    float dotNH = clamp(dot(normalDir, halfDir), 0.0, 1.0);

    vec3 diffuse = color * dotNL;
    vec3 specular = light1Intensity * pow(dotNH, 10.0);

    outColor = vec4(diffuse + specular, 1);
}
