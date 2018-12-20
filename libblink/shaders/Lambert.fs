R"===(
#version 330 core

uniform vec3 u_lightPos;
uniform vec3 u_viewPos;
uniform vec3 u_lightColor;
uniform vec3 u_ambientColor;
uniform vec3 u_diffuseColor;

in vec2 v_texCoord;
in vec3 v_normal;
in vec3 v_fragPos;

void main ()
{
    vec3 fragNormal = normalize(v_normal);
    vec3 lightDir = normalize(u_lightPos - v_fragPos);
    vec3 viewDir = normalize(u_viewPos - v_fragPos);
	
	// ambient
	
	// diffuse
    float diffuseComponent = max(dot(fragNormal, lightDir), 0.0);
    vec3 diffuseColor = diffuseComponent * u_diffuseColor * u_lightColor;
	
	// specular
    vec3 reflectDir = reflect(-lightDir, fragNormal);
    float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    vec3 specularColor = 0.5 * specularComponent * u_lightColor;
	
    gl_FragColor = vec4(u_ambientColor + diffuseColor + specularColor, 1.0);
}
)==="