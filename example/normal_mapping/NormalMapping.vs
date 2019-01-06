R"===(
uniform mat4 u_worldToClip;
uniform mat4 u_localToWorld;
uniform mat3 u_localToWorldTranInv;
uniform mat4 u_localToClip;

uniform vec3 u_lightPos;
uniform vec3 u_viewPos;

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_texCoord;
layout (location = 2) in vec3 a_normal;
layout (location = 3) in vec3 a_tangent;

out vec2 f_texCoord;
out vec3 f_lightPos;
out vec3 f_viewPos;
out vec3 f_fragPos;

void main()
{
    gl_Position = u_localToClip * vec4(a_position, 1.0);
    vec3 fragPosInWorldSpace = vec3(u_localToWorld * vec4(a_position, 1.0));
    f_texCoord = a_texCoord;

    mat3 localToWorldTranInv = transpose(inverse(mat3(u_localToWorld)));
    vec3 T = normalize(localToWorldTranInv * a_tangent);
    vec3 N = normalize(localToWorldTranInv * a_normal);
	
	// re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
	
	// then retrieve perpendicular vector B with the cross product of T and N
    vec3 B = cross(N, T);

    mat3 worldToTangent = transpose(mat3(T, B, N));
    f_lightPos = worldToTangent * u_lightPos;
    f_viewPos = worldToTangent * u_viewPos;
	f_fragPos = worldToTangent * fragPosInWorldSpace;
}
)==="
