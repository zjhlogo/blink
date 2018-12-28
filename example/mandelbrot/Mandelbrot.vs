R"===(
uniform mat4 u_worldToClip;
uniform mat4 u_localToWorld;
uniform mat3 u_localToWorldTranInv;
uniform mat4 u_localToClip;

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texCoord;
layout (location = 2) in vec3 a_normal;

out vec2 v_position;

void main()
{
    gl_Position = u_localToClip * vec4(a_position, 1.0);
	v_position = a_position.xy;
}
)==="
