R"===(
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 f_barycentric;

void main()
{
	f_barycentric = vec3(1.0, 0.0, 0.0);
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	
	f_barycentric = vec3(0.0, 1.0, 0.0);
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	f_barycentric = vec3(0.0, 0.0, 1.0);
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();

    EndPrimitive();
}
)==="
