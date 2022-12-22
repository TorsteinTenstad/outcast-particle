#version 140

uniform vec2 player_pos;
uniform vec2 other_pos;
uniform vec2 window_resolution;

#define PI 3.1415926535897932384626433832795

float atan01(float x, float y)
{
	return (atan(x, y) / PI + 1) / 2;
}

void main()
{
	vec2 pos = gl_FragCoord.xy;
	pos.y = window_resolution.y - pos.y;

	mat2 quarter_rotation = mat2(0, 1, -1, 0);

	mat2 basis;
	basis[0] = vec2(other_pos - player_pos) / 2;
	basis[1] = quarter_rotation * basis[0];
	basis[1] *= 50 / length(basis[0]);

	pos = inverse(basis) * (pos - player_pos - basis[0]);

	pos = abs(pos);
	float mask = float(pos.x <= 1 && pos.y <= 1);
	pos *= mask;
	float shape = (1 - pos.y) * (0.2 + 0.8 * pow(pos.x, 4));
	shape *= mask;
	gl_FragColor = vec4(vec3(shape), 0.5 * float(shape > 0.15));
	return;

	float distance_from_player = length(pos - player_pos);
	float distance_from_other = length(pos - other_pos);
	vec2 u = pos - other_pos;
	vec2 v = pos - player_pos;
	float angle_diff = abs(atan01(u.x, u.y) - atan01(v.x, v.y));
	if (angle_diff > 0.5)
	{
		angle_diff = 1 - angle_diff;
	}
	float player_proximity = 1 - 3 * distance_from_player / length(window_resolution);
	float other_proximity = 1 - 3 * distance_from_other / length(window_resolution);
	player_proximity = max(player_proximity, 0);
	other_proximity = max(other_proximity, 0);
	float d = max(player_proximity, other_proximity);
	gl_FragColor = vec4(0, fract(d), 0, 1.0);
}