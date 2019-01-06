R"===(
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;

uniform sampler2D tex_cartoonLevel;

in vec3 f_normal;
in vec3 f_fragPos;

void main ()
{
    vec3 fragNormal = normalize(f_normal);
    vec3 lightDir = normalize(u_lightPos - f_fragPos);
	
	// diffuse
    float diffuseComponent = max(dot(fragNormal, lightDir), 0.0);
    gl_FragColor = texture(tex_cartoonLevel, vec2(diffuseComponent, 0.5f));
}
)==="
