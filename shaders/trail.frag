#version 120

#define PI 3.1415926535897932384626433832795
#define MAX_N 64

uniform int segment_last_updated;
uniform int segments_created;
uniform float radius;
uniform vec2 path[MAX_N];

void main()
{
	vec2 uv = gl_TexCoord[0].xy;

	vec4 final_color = vec4(1, 1, 1, 0);

	for (int i = 0; i < segments_created; i++)
	{
		int remapped_i = segment_last_updated - i;
		remapped_i = remapped_i < 0 ? remapped_i + segments_created : remapped_i;
		vec2 pos = (path[remapped_i]);

		float gradient = pow(float(segments_created-i-1)/segments_created, 2);
		float AA = 1;
		float alpha = 0.8*gradient*smoothstep(0.f, AA, 0.7*radius*gradient-length(uv-pos));
		vec4 color = vec4(mix(vec3(1, 1, 1), vec3(1, 1, 1), gradient), alpha);
		if (alpha > final_color.a){
			final_color = color;
		}
	}
	gl_FragColor = gl_Color*final_color;
	//gl_FragColor = vec4(fract(uv/120), 1, 0.25);
}