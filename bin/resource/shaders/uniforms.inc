layout(set = 0, binding = 0) uniform PerFrameUniforms
{
    mat4 matWorldToCamera;
    mat4 matCameraToProjection;
    mat4 matWorldToProjection;
    mat3 matWorldToCameraInvT;
    vec3 cameraPos;
    vec3 cameraDir;
    vec3 lightPos;
    vec4 lightColorAndIntensity;
} pfu;

layout(set = 0, binding = 1) uniform PerInstanceUniforms
{
    mat4 matLocalToWorld;
    mat3 matLocalToWorldInvT;
} piu;
