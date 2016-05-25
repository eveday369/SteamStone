uniform vec3 ambient;
uniform vec3 lightDiffuse;
uniform sampler2D diffuseMap;
uniform sampler2D bumpMap;

varying vec2 texcoord;

void main( void )
{
	vec4 diffuseTex = texture2D(diffuseMap, texcoord);
	vec3 normal = normalize(texture2D(bumpMap, texcoord).xyz * 4.0 - 1.0);
	vec3 color = (ambient * diffuseTex.xyz) + max(dot(normal, lightDiffuse), 0.0) * diffuseTex.xyz;
	gl_FragColor = vec4(color, diffuseTex.w);
}
