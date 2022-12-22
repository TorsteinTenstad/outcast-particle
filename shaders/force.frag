#version 140

#define PI 3.1415926535897932384626433832795
#define MAX_CHARGES 64
#define QUARTER_TURN mat2(0, 1, -1, 0)

uniform int n_charges;
uniform float charge_radius;
uniform vec2 charges[MAX_CHARGES];

uniform vec2 player_pos;
uniform vec2 window_resolution;

float atan01(float x, float y)
{
	return (atan(x, y) / PI + 1) / 2;
}

void main()
{
	vec2 p = gl_FragCoord.xy;
	p.y = window_resolution.y - p.y;

	vec2 val = vec2(0, 0);

	for (int i = 0; i < n_charges; i++)
	{
		mat2 basis;
		basis[0] = vec2(charges[i].xy - player_pos) / 2;
		basis[1] = QUARTER_TURN * basis[0];
		basis[1] *= charge_radius / length(basis[0]);

		vec2 val_contribution = inverse(basis) * (p - player_pos - basis[0]);

		val_contribution = abs(val_contribution);
		float mask = float(val_contribution.x <= 1 && val_contribution.y <= 1);
		val_contribution *= mask;

		val += val_contribution;
	}

	gl_FragColor = vec4(val, 0, float(val.x > 0 && val.y > 0));
	return;
}
/*
	float shape = (1 - p.y) * (0.2 + 0.8 * pow(p.x, 4));
	shape *= mask;
	gl_FragColor = vec4(vec3(shape), 0.5 * float(shape > 0.15));
	return;
}
*/