layout(set = 0, binding = 0) uniform FrameUniforms
{
    vec3 lightPos;
    float time;
    vec3 lightColor;
    float lightIntensity;
    vec2 screenResolution;
} fu;

layout(set = 0, binding = 1) uniform CameraUniforms
{
    mat4 matWorldToCamera;
    mat4 matCameraToProjection;
    mat4 matWorldToProjection;
    mat3 matWorldToCameraInvT;
    vec3 cameraPos;
    vec3 cameraDir;
} cu;

layout(push_constant) uniform PushConstantData
{
    mat4 matLocalToWorld;
    mat3 matLocalToWorldInvT;
} pushData;
