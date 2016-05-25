uniform sampler2D TEX0;
// could use a vec3 for single parameter RGB, however i prefer 3 separated floats to update the value.
uniform float red;
uniform float green;
uniform float blue;

varying vec2 uv;

void main()
{
	vec4 c = texture2D(TEX0, uv);
	vec4 l = vec4(red, green, blue, 0.0);

	gl_FragColor = vec4(
		1.0 - ( (1.0 - l.x) * (1.0 - c.x) ),
		1.0 - ( (1.0 - l.y) * (1.0 - c.y) ),
		1.0 - ( (1.0 - l.z) * (1.0 - c.z) ),
		0.0);
}
