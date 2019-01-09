R"===(
uniform mat4 u_worldToClip;
uniform mat4 u_localToWorld;
uniform mat3 u_localToWorldTranInv;
uniform mat4 u_localToClip;

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texCoord;

out vec2 f_texCoord;

void main()
{
    gl_Position = u_localToClip * vec4(a_position, 1.0);
	f_texCoord = a_texCoord;
}
)==="
