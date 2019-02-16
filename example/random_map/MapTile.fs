R"===(
uniform vec3 u_viewPos;
uniform sampler2D tex_diffuse;

in vec2 f_texCoord;

void main ()
{
    gl_FragColor = texture(tex_diffuse, f_texCoord);
}
)==="
