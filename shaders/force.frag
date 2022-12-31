#version 140

#define PI 3.1415926535897932384626433832795
#define MAX_CHARGES 64
#define QUARTER_TURN mat2(0, 1, -1, 0)
#define MAX_FORCE pow(10000 / 240, 2)
#define FORCE_THREASHOLD_NEEDED_TO_CONNECT pow(10000 / (5 * 240), 2)

uniform float _time;
uniform int n_charges;
uniform float charge_radius;
uniform vec2 charge_positions[MAX_CHARGES];
uniform float charge_force[MAX_CHARGES];

uniform vec2 player_pos;
uniform vec2 window_resolution;

float atan01(float x, float y)
{
	return (atan(x, y) / PI + 1) / 2;
}

void main()
{
	vec2 screen_coords = gl_FragCoord.xy;
	screen_coords.y = window_resolution.y - screen_coords.y;

	vec4 color = vec4(0, 0, 0, 0);

	for (int i = 0; i < n_charges; i++)
	{
		//if (abs(charge_force[i]) > FORCE_THREASHOLD_NEEDED_TO_CONNECT)
		{
			mat2 basis;
			basis[0] = vec2(charge_positions[i].xy - player_pos) / 2;
			basis[1] = QUARTER_TURN * basis[0];
			basis[1] *= 0.5 * charge_radius / length(basis[0]);

			vec2 local_coords = inverse(basis) * (screen_coords - player_pos - basis[0]);

			local_coords.y = abs(local_coords.y);
			float square_mask = float(-1 <= local_coords.x && local_coords.x <= 1 && local_coords.y <= 1);

			local_coords *= square_mask;
			float rescaled_x = local_coords.x * length(basis[0]);

			float a = abs(charge_force[i]) / MAX_FORCE;

			float flow_dir = (abs(charge_force[i]) / charge_force[i]);

			float wave_min = 0.5;
			float wave_max = 1;
			float wave_amplitude = (wave_max - wave_min);
			float wave_screen_space_frequency = 0.05;
			float wave_temporal_frequency = 20;
			float wave_discard_peaks = 4;

			float spatial_theta = wave_screen_space_frequency * rescaled_x;
			float theta = spatial_theta + 5 * _time * flow_dir - 0.025 * charge_radius * local_coords.y * flow_dir;
			theta *= float(fract(theta / wave_discard_peaks) > (wave_discard_peaks - 1) / (wave_discard_peaks));

			float arrows = wave_min + wave_amplitude * (1 - pow(sin(PI * fract(theta) - PI / 2), 6));
			vec4 color_contribution = vec4(vec3(arrows), a * min(1, abs(10 * (local_coords.y - 1))));
			color_contribution *= square_mask;

			if (color.a < color_contribution.a)
			{
				color = color_contribution;
			}
		}
	}

	gl_FragColor = color;
	return;
}
/*
	float shape = (1 - screen_coords.y) * (0.2 + 0.8 * pow(screen_coords.x, 4));
	shape *= square_mask;
	gl_FragColor = vec4(vec3(shape), 0.5 * float(shape > 0.15));
	return;
}
*/