R"===(
uniform mat4 u_worldToClip;
uniform mat4 u_localToWorld;
uniform mat3 u_localToWorldTranInv;
uniform mat4 u_localToClip;

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texCoord;
layout (location = 2) in vec3 a_normal;

out vec2 f_texCoord;
out vec3 f_normal;
out vec3 f_fragPos;

void main()
{
    gl_Position = u_localToClip * vec4(a_position, 1.0);
    f_fragPos = vec3(u_localToWorld * vec4(a_position, 1.0));
    f_normal = u_localToWorldTranInv * a_normal;
    f_texCoord = a_texCoord;
}
)==="
