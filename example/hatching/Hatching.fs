R"===(
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;
uniform vec3 u_lightColor;
uniform vec3 u_ambientColor;
uniform vec3 u_diffuseColor;

uniform sampler2D tex_diffuse;
uniform sampler2D tex_hatchingLevel;

in vec2 f_texCoord;
in vec3 f_normal;
in vec3 f_fragPos;

void main ()
{
    vec3 fragNormal = normalize(f_normal);
    vec3 lightDir = normalize(u_lightPos - f_fragPos);
	
	// ambient
	
	// diffuse
    float diffuseComponent = max(dot(fragNormal, lightDir), 0.0);
	vec4 hatchingFactor = texture(tex_hatchingLevel, vec2(diffuseComponent, 0.5f));

	vec4 diffuseColor = texture(tex_diffuse, f_texCoord);
	vec4 final = diffuseColor * hatchingFactor;

	float color = final.r + final.g + final.b;
    gl_FragColor = vec4(color, color, color, 1.0f);
}
)==="
