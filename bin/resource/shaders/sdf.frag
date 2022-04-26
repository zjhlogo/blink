#version 450
#extension GL_ARB_separate_shader_objects : enable

#include "builtin_uniforms.inc"

layout(location = 0) out vec4 outColor;

void main()
{
    vec2 st = (gl_FragCoord.xy * 2.0 - fu.screenResolution) / fu.screenResolution.y;
    st.y = -st.y;
    outColor = vec4(st.x, st.y, 0, 1.0);
}
