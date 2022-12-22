#version 140

#define PI 3.1415926535897932384626433832795
#define MAX_CHARGES 64
#define QUARTER_TURN mat2(0, 1, -1, 0)
#define MAX_FORCE pow(10000 / 240, 2)
#define FORCE_THREASHOLD_NEEDED_TO_CONNECT pow(10000 / (5 * 240), 2)

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

	vec3 color = vec3(0, 0, 0);

	for (int i = 0; i < n_charges; i++)
	{
		if (abs(charge_force[i]) > FORCE_THREASHOLD_NEEDED_TO_CONNECT)
		{
			mat2 basis;
			basis[0] = vec2(charge_positions[i].xy - player_pos) / 2;
			basis[1] = QUARTER_TURN * basis[0];
			basis[1] *= charge_radius / length(basis[0]);

			vec2 local_coords = inverse(basis) * (screen_coords - player_pos - basis[0]);

			local_coords = abs(local_coords);
			float square_mask = float(local_coords.x <= 1 && local_coords.y <= 1);

			local_coords *= square_mask;
			float a = (abs(charge_force[i])) / (MAX_FORCE);
			float invert = 0.5 - 0.5 * sign(charge_force[i]);
			//invert + sign(charge_force[i]) * pow(local_coords.x, 2)
			vec3 color_contribution = vec3(0.5 + 0.5 * pow(local_coords.x, 2)) * float((1 - a) * pow(local_coords.x, 4) + a > abs(local_coords.y));
			color_contribution *= square_mask;

			color += color_contribution;
		}
	}

	gl_FragColor = vec4(vec3(color), 0.25 * float(length(color) > 0));
	return;
}
/*
	float shape = (1 - screen_coords.y) * (0.2 + 0.8 * pow(screen_coords.x, 4));
	shape *= square_mask;
	gl_FragColor = vec4(vec3(shape), 0.5 * float(shape > 0.15));
	return;
}
*/