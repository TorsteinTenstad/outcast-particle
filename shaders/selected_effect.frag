#version 120

#define MAX_ARRAY_SIZE 64
#define BLOCK_SIZE 120

uniform vec2 positions[MAX_ARRAY_SIZE];
uniform vec2 sizes[MAX_ARRAY_SIZE];
uniform int n;

float Box(vec2 uv, vec2 position, vec2 size, float AA)
{
	vec2 center_offset = position - uv;
	vec2 sd = abs(center_offset) - size / 2.f;
	float max_sd = max(sd.x, sd.y);
	float box = 1 - smoothstep(0, AA, abs(max_sd + (BLOCK_SIZE / 16)) - BLOCK_SIZE / 16);
	return box;
}

vec4 ColoredBox(vec2 uv, vec2 position, vec2 size)
{
	float AA = 3;
	vec3 blue = vec3(0, 0, 1);
	float fill_box = Box(uv, position, size, AA);
	vec2 checkered_pattern_g_uv = floor(12 * uv / float(BLOCK_SIZE));
	float checkered_pattern = 2 * fract((checkered_pattern_g_uv.x + checkered_pattern_g_uv.y) / 2);

	vec4 fill = vec4(blue, fill_box);
	return fill;
}

void main()
{
	vec2 uv = gl_TexCoord[0].xy;

	vec4 color = vec4(0);

	for (int i = 0; i < min(n, MAX_ARRAY_SIZE); i++)
	{
		vec4 color_contribution = ColoredBox(uv, positions[i], sizes[i]);
		if (color_contribution.a > 0.0001)
		{
			color = color_contribution;
			break;
		}
	}

	gl_FragColor = color;
}