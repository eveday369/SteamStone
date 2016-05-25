uniform sampler2D TEX0;
varying vec2 uv;

void main()
{
	gl_FragColor = texture2D(TEX0, uv);
}
