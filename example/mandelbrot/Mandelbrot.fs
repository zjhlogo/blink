R"===(
uniform float u_maxIterations;
uniform float u_zoom;
uniform float u_escapeRadius;
uniform vec2 u_centerPos;
uniform vec3 u_colorScale;

in vec2 v_position;

float mandelbrot()
{
	vec2 c = u_centerPos + v_position*u_zoom;
	vec2 z = c;

	float n = 0.0;
	for (; n < u_maxIterations; ++n)
	{
		z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + c;
		float modulus = length(z);
		if(modulus > u_escapeRadius)
		{
			return (n + 1.0 - log(log(modulus))/log(2.0));	// http://linas.org/art-gallery/escape/escape.html
		}
	}
	return 0.0;
}

void main ()
{
    float n = mandelbrot();
    gl_FragColor = vec4((-cos(u_colorScale*n)+1.0)/2.0, 1.0);
//    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
)==="
