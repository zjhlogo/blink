#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "builtin_uniforms.inc"

layout(location = 0) out vec4 outColor;

void main()
{
    outColor = vec4(gl_FragCoord.x/fu.screenResolution.x, gl_FragCoord.y/fu.screenResolution.y, gl_FragCoord.z, 1.0);
}
