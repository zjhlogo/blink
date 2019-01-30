R"===(
uniform vec3 u_diffuseColor;

in vec3 f_barycentric;

float edgeFactor()
{
	float minValue = min(min(f_barycentric.x, f_barycentric.y), f_barycentric.z);
    float d = fwidth(minValue);
    return smoothstep(0.0, d, minValue);
}

void main ()
{
	gl_FragColor = vec4(mix(vec3(0.0), u_diffuseColor, edgeFactor()), 1.0);
}
)==="
