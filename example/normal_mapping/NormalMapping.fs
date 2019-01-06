R"===(
uniform vec3 u_lightColor;
uniform vec3 u_ambientColor;
uniform vec3 u_diffuseColor;

uniform sampler2D tex_diffuse;
uniform sampler2D tex_normal;

in vec2 f_texCoord;
in vec3 f_lightPos;
in vec3 f_viewPos;
in vec3 f_fragPos;

void main ()
{
    // obtain normal from normal map in range [0,1]
    vec3 fragNormal = texture(tex_normal, f_texCoord).rgb;
	
    // transform normal vector to range [-1,1]
    fragNormal = normalize(fragNormal * 2.0 - 1.0);  // this normal is in tangent space

    vec3 lightDir = normalize(f_lightPos - f_fragPos);
    vec3 viewDir = normalize(f_viewPos - f_fragPos);
	
	// ambient
	
	// diffuse
    float diffuseComponent = max(dot(fragNormal, lightDir), 0.0);
	vec3 texDiffuse = texture(tex_diffuse, f_texCoord).xyz;
    vec3 diffuseColor = diffuseComponent * u_diffuseColor * u_lightColor * texDiffuse;
	
	// specular
    vec3 reflectDir = reflect(-lightDir, fragNormal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float specularComponent = pow(max(dot(fragNormal, halfwayDir), 0.0), 32.0);
    vec3 specularColor = 0.5 * specularComponent * u_lightColor;
	
    gl_FragColor = vec4(u_ambientColor + diffuseColor + specularColor, 1.0);
}
)==="
