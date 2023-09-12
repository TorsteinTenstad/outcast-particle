#version 120

#include "game_data\\shaders\\include\\math_utils.glsl";

#define MAX_N 64

uniform int segment_last_updated;
uniform int segments_created;
uniform float radius;
uniform vec2 path[MAX_N];
uniform vec2 origin;

#define QUARTER_TURN mat2(0, 1, -1, 0)

void main()
{
	vec2 uv = gl_TexCoord[0].xy;

	vec4 final_color = vec4(1, 1, 1, 0);

	vec2 p0 = origin;
	vec2 p1 = origin;
	float accumulated_length = 0;

	for (int i = 0; i < segments_created - 1; i++)
	{
		int i0 = segment_last_updated - i - 0;
		i0 = i0 < 0 ? i0 + segments_created : i0;
		vec2 i_delta = path[i0];
		p0 = p1;
		p1 += i_delta;
		
		float l = length(i_delta);
		accumulated_length += l;
		mat2 basis;
		basis[0] = i_delta/l;
		basis[1] = QUARTER_TURN * basis[0];

		vec2 uv_transformed = inverse(basis)*(uv-p0);
		vec2 closest = vec2(clamp(uv_transformed.x, 0, l), 0);
		vec2 dist = uv_transformed-closest;
		float d = length(dist);

		float gradient = pow(float(segments_created-i-closest.x/l)/segments_created, 2);
		gradient = clamp(gradient, 0, 1);
		float AA = 4;
		float alpha = 0.8*smoothstep(0.f, AA, 0.8*radius*gradient-d)*smoothstep(3*radius+l*8, 0, accumulated_length+uv_transformed.x);
		vec4 color = vec4(mix(vec3(1, 1, 1), vec3(1, 1, 1), gradient), alpha);
		if (alpha > final_color.a){
			final_color = color;
		}
		//final_color += vec4(1, 0, 0, 1-smoothstep(0, 1, length(uv-p1) - 5));
	}
	gl_FragColor = gl_Color*final_color;
	//gl_FragColor = vec4(fract(uv/120), 1, 0.25);
}