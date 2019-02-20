R"===(
uniform vec3 u_viewPos;
uniform sampler2D tex_diffuse;

in vec2 f_texCoord;

void main ()
{
//    gl_FragColor = vec4(f_texCoord.x, f_texCoord.x, f_texCoord.x, 1.0);
    vec4 color = texture(tex_diffuse, f_texCoord);
    gl_FragColor = vec4(color.xyz, 1.0);
}
)==="
