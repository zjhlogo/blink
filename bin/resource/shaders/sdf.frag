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
const float MAX_DIST = 2.0;
const float EPSILON = 0.001;

float sdf_sphere(vec3 p, float r)
{
    return length(p) - r;
}

float sdf_box(vec3 p, vec3 b)
{
    vec3 q = abs(p) - b;
    return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float sdf_torus(vec3 p, vec2 t)
{
    vec2 q = vec2(length(p.xy) - t.x, p.z);
    return length(q) - t.y;
}

float get_dist(vec3 p)
{
    float dist = sdf_sphere(p - vec3(0, 0, 0), 1.0);
    return dist;
    // float dist2 = sdf_torus(p - vec3(0.3, 0, 0), vec2(0.1, 0.02));
    // float dist3 = sdf_box(p, vec3(0.1, 0.1, 0.1));
    // return min(min(dist, dist2), dist3);
}

vec3 get_normal(vec3 p)
{
    return normalize(vec3(
      get_dist(vec3(p.x + EPSILON, p.y, p.z)) - get_dist(vec3(p.x - EPSILON, p.y, p.z)),
      get_dist(vec3(p.x, p.y + EPSILON, p.z)) - get_dist(vec3(p.x, p.y - EPSILON, p.z)),
      get_dist(vec3(p.x, p.y, p.z  + EPSILON)) - get_dist(vec3(p.x, p.y, p.z - EPSILON))
    ));
}

float ray_match(vec3 rayStart, vec3 rayDir)
{
    float depth = 0;

    for (int i = 0; i < MAX_STEPS; ++i)
    {
        vec3 p = rayStart + rayDir * depth;
        float dist = get_dist(p);

        depth += dist;
        if (depth > MAX_DIST || dist < EPSILON) break;
    }

    return depth;
}

vec3 calcRayDir(float halfFov)
{
    vec2 xy = gl_FragCoord.xy - fu.screenResolution * 0.5;
    float z = gl_FragCoord.y / tan(halfFov);
    return normalize(vec3(xy, -z));
}

void main()
{
    vec3 dir = calcRayDir(mu.fov);
    outColor = vec4(dir.x, dir.y, -dir.z, 1);
    // vec3 eye = vec3(0.0, 0.0, 5.0);
    
    // float dist = ray_match(eye, dir);
    // if (dist > MAX_DIST - EPSILON)
    // {
    //     outColor = vec4(0, 0, 0, 1);
    //     return;
    // }

    // outColor = vec4(1, 0, 0, 1);
}
