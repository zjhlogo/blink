#pragma once

const char* LAMBERT_VS =
"#version 330 core\n\
uniform mat4 u_worldToClip; \
uniform mat4 u_localToWorld; \
uniform mat4 u_localToClip; \
layout (location = 0) in vec3 a_position; \
layout (location = 1) in vec2 a_texCoord; \
layout (location = 2) in vec3 a_normal; \
out vec2 v_texCoord; \
out vec3 v_normal; \
out vec3 v_fragPos; \
void main() \
{ \
    gl_Position = u_localToClip * vec4(a_position, 1.0); \
    v_fragPos = vec3(u_localToWorld * vec4(a_position, 1.0)); \
    v_texCoord = a_texCoord; \
    v_normal = mat3(transpose(inverse(u_localToWorld))) * a_normal; \
}";

const char* LAMBERT_FS =
"#version 330 core\n\
uniform vec3 u_lightPos; \
uniform vec3 u_viewPos; \
uniform vec3 u_lightColor; \
uniform vec3 u_ambientColor; \
uniform vec3 u_diffuseColor; \
in vec2 v_texCoord; \
in vec3 v_normal; \
in vec3 v_fragPos; \
void main () \
{ \
    vec3 fragNormal = normalize(v_normal); \
    vec3 lightDir = normalize(u_lightPos - v_fragPos); \
    vec3 viewDir = normalize(u_viewPos - v_fragPos); \
    float diffuseComponent = max(dot(fragNormal, lightDir), 0.0); \
    vec3 diffuseColor = diffuseComponent * u_diffuseColor * u_lightColor; \
    vec3 reflectDir = reflect(-lightDir, fragNormal); \
    float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), 128); \
    vec3 specularColor = 0.5 * specularComponent * u_lightColor; \
    gl_FragColor = vec4(u_ambientColor + diffuseColor + specularColor, 1.0); \
}";
