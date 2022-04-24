#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;

void main()
{
    outColor = vec4(gl_FragCoord.x/1280, gl_FragCoord.y/720, gl_FragCoord.z, 1.0);
}
