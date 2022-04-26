layout(set = 0, binding = 0) uniform PerCameraUniforms
{
    mat4 matWorldToCamera;
    mat3 matWorldToCameraInvT;
    mat4 matCameraToProjection;
    mat4 matWorldToProjection;
    vec3 cameraPos;
    vec3 cameraDir;
    vec3 lightPos;
    vec4 lightColorAndIntensity;
} pcu;

layout(set = 0, binding = 1) uniform PerInstanceUniforms
{
    mat4 matLocalToWorld;
    mat3 matLocalToWorldInvT;
    mat4 matLocalToProjection;
} piu;
