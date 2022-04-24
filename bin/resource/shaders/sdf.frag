#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

float sdf_sphere(vec3 p, float s)
{
    return length(p)-s;
}

vec3 get_vertex_on_sphere_surface(vec3 center, float radius)
{
    vec2 dist = fragTexCoord - center.xy;
    float sqrXY = dot(dist, dist);
    float sqrR = radius * radius;
    float sqrZ = clamp(sqrR - sqrXY, 0.0, sqrR);
    return vec3(fragTexCoord.x, fragTexCoord.y, sqrt(sqrZ) + center.z);
}

void main()
{
    vec3 lightDir = vec3(0.577, 0.577, 0.577);
    vec3 center = vec3(0.5, 0.5, 0.0);
    float radius = 0.1;
    
    vec3 vertex = get_vertex_on_sphere_surface(center, radius);
    vec3 dist = vertex - center;
    vec3 normal = normalize(dist);
    // float sdf = sdf_sphere(dist, radius);
    // float result = smoothstep(-fwidth(sdf), 0.0, sdf);
    float result = dot(normal, lightDir);
    outColor = vec4(result, result, result, 1.0);
}
