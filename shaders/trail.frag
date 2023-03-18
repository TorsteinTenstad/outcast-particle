#version 120

#define PI 3.1415926535897932384626433832795
#define MAX_N 64

uniform int segment_last_updated;
uniform int segments_created;
uniform float radius;
uniform vec2 path[MAX_N];

#define QUARTER_TURN mat2(0, 1, -1, 0)
mat2 inverse(mat2 m) {
  return mat2(m[1][1],-m[0][1],
             -m[1][0], m[0][0]) / (m[0][0]*m[1][1] - m[0][1]*m[1][0]);
}

void main()
{
	vec2 uv = gl_TexCoord[0].xy;

	vec4 final_color = vec4(1, 1, 1, 0);

	for (int i = 0; i < segments_created - 1; i++)
	{
		int i0 = segment_last_updated - i - 0;
		int i1 = segment_last_updated - i - 1;
		i0 = i0 < 0 ? i0 + segments_created : i0;
		i1 = i1 < 0 ? i1 + segments_created : i1;
		vec2 p0 = (path[i0]);
		vec2 p1 = (path[i1]);
		
		vec2 l = p1 - p0;
		mat2 basis;
		basis[0] = l/length(l);
		basis[1] = QUARTER_TURN * basis[0];

		vec2 uv_transformed = inverse(basis)*(uv-p0);
		vec2 closest = vec2(clamp(uv_transformed.x, 0, length(l)), 0);
		float d = length(uv_transformed-closest);

		float gradient = pow(float(segments_created-i)/segments_created, 2);
		gradient = clamp(gradient, 0, 1);
		float AA = 1;
		float alpha = 0.8*gradient*smoothstep(0.f, AA, 0.7*radius*gradient-d);
		vec4 color = vec4(mix(vec3(1, 1, 1), vec3(1, 1, 1), gradient), alpha);
		if (alpha > final_color.a){
			final_color = color;
		}
		//final_color += vec4(1, 0, 0, 1-smoothstep(0, 1, length(uv-p1) - 10));
	}
	gl_FragColor = gl_Color*final_color;
	//gl_FragColor = vec4(fract(uv/120), 1, 0.25);
}