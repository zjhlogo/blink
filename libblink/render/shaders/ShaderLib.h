#pragma once

const char* LAMBER_VS =
"uniform  mat4 u_localToClip; \
attribute vec3 a_position; \
attribute vec2 a_texCoord; \
attribute vec3 a_normal; \
varying vec2 v_texCoord; \
varying vec3 v_normal; \
void main() \
{ \
    gl_Position = u_localToClip * vec4(a_position, 1.0); \
    v_texCoord = a_texCoord; \
    v_normal = a_normal; \
}";

const char* LAMBER_FS =
"uniform vec3 u_lightDir; \
varying vec2 v_texCoord; \
varying vec3 v_normal; \
void main () \
{ \
    vec3 fragNormal = normalize(v_normal); \
    float diffuseComponent = clamp(dot(fragNormal, u_lightDir), 0.0, 1.0); \
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0); \
}";
