#version 120

#define PI 3.1415926535897932384626433832795
#define MAX_ARRAY_SIZE 64
#define QUARTER_TURN mat2(0, 1, -1, 0)
#define MAX_FORCE pow(10000 / 240, 2)
#define FORCE_THREASHOLD_NEEDED_TO_CONNECT pow(10000 / (5 * 240), 2)

uniform float _time;
uniform vec2 _wh;
uniform vec2 _window_resolution;

uniform vec2 player_pos;

uniform int n_charges;
uniform float charge_radius;
uniform vec2 charge_positions[MAX_ARRAY_SIZE];
uniform float charge_force[MAX_ARRAY_SIZE];

mat2 inverse(mat2 m)
{
	return mat2(m[1][1], -m[0][1], -m[1][0], m[0][0]) / (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
}

float atan01(float x, float y)
{
	return (atan(x, y) / PI + 1) / 2;
}

float inverse_mix(float a, float b, float t)
{
	float t_clamped = clamp(t, a, b);
	return (t_clamped - a) / (b - a);
}

float soft_threshold(float t, float threshold, float softness)
{
	return mix(0, 1, inverse_mix(threshold - 0.5 * softness, threshold + 0.5 * softness, t));
}
void main()
{
	vec2 xy = gl_TexCoord[0].xy*_wh;

	vec4 color = vec4(0, 0, 0, 0);

	for (int i = 0; i < min(float(n_charges), float(MAX_ARRAY_SIZE)); i++)
	{
		mat2 basis;
		basis[0] = vec2(charge_positions[i].xy - player_pos) / 2;
		basis[1] = QUARTER_TURN * basis[0] / length(basis[0]);
		basis[1] *= 0.5 * charge_radius;

		vec2 local_coords = inverse(basis) * (xy - player_pos - basis[0]);

		local_coords.y = abs(local_coords.y);
		float square_mask = float(-1 <= local_coords.x && local_coords.x <= 1 && local_coords.y <= 1);

		local_coords *= square_mask;
		float rescaled_x = local_coords.x * length(basis[0]) / charge_radius;

		float a = abs(charge_force[i]) / MAX_FORCE;

		float flow_dir = (abs(charge_force[i]) / charge_force[i]);

		float background_color = 0.5;
		float arrow_color = 1;
		float arrows_per_radius_length = 0.5;
		float arrow_width = 0.2;
		float wave_temporal_frequency = 20;
		float wave_discard_peaks = 4;

		float x_component = arrows_per_radius_length * rescaled_x;
		float y_component = -0.2 * local_coords.y * flow_dir;
		float temporal_component = 1.5 * _time * flow_dir;
		float t = abs(fract(x_component + y_component + temporal_component) - 0.5) * 2;
		float AA_softness = 0.02;
		t = soft_threshold(t, 1 - arrow_width, AA_softness);
		float color_rgb = mix(background_color, arrow_color, t);
		vec4 color_contribution = vec4(vec3(color_rgb), a * soft_threshold(1 - local_coords.y, AA_softness / 2, AA_softness));
		color_contribution *= square_mask;

		if (color.a < color_contribution.a)
		{
			color = color_contribution;
		}
	}
	color.a = clamp(color.a, 0, 0.5);
	gl_FragColor = gl_Color * color;
	return;
}
/*
	float shape = (1 - xy.y) * (0.2 + 0.8 * pow(xy.x, 4));
	shape *= square_mask;
	gl_FragColor = vec4(vec3(shape), 0.5 * float(shape > 0.15));
	return;
}
*/