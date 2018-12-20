R"===(
#version 330 core
uniform mat4 u_worldToClip;
uniform mat4 u_localToWorld;
uniform mat3 u_localToWorldTranInv;
uniform mat4 u_localToClip;

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texCoord;
layout (location = 2) in vec3 a_normal;

out vec2 v_texCoord;
out vec3 v_normal;
out vec3 v_fragPos;

void main()
{
    gl_Position = u_localToClip * vec4(a_position, 1.0);
    v_fragPos = vec3(u_localToWorld * vec4(a_position, 1.0));
    v_normal = u_localToWorldTranInv * a_normal;
    v_texCoord = a_texCoord;
}
)==="
