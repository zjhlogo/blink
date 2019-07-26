R"===(
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;
uniform vec3 u_lightColor;
uniform vec3 u_ambientColor;
uniform vec3 u_diffuseColor;

#ifdef USE_DIFFUSE_TEXTURE
uniform sampler2D tex_diffuse;
#endif

in vec2 f_texCoord;
in vec3 f_normal;
in vec3 f_fragPos;

void main ()
{
    vec3 fragNormal = normalize(f_normal);
    vec3 lightDir = normalize(u_lightPos - f_fragPos);
    vec3 viewDir = normalize(u_viewPos - f_fragPos);
	
	// ambient
	
	// diffuse
    float diffuseComponent = max(dot(fragNormal, lightDir), 0.0);
	
#ifdef USE_DIFFUSE_TEXTURE
	vec3 texDiffuse = texture(tex_diffuse, f_texCoord).xyz;
    vec3 diffuseColor = diffuseComponent * u_diffuseColor * u_lightColor * texDiffuse;
#else
    vec3 diffuseColor = diffuseComponent * u_diffuseColor * u_lightColor;
#endif
	
	// specular
    vec3 reflectDir = reflect(-lightDir, fragNormal);
    float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    vec3 specularColor = 0.5 * specularComponent * u_lightColor;
	
    gl_FragColor = vec4(u_ambientColor + diffuseColor + specularColor, 1.0);
}
)==="
