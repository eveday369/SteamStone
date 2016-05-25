uniform sampler2D TEX0;
uniform sampler2D TEX1;

varying vec2 uv;

void main()
{
	gl_FragColor = texture2D(TEX0, uv) * texture2D(TEX1, uv);
}
