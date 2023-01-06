#version 140

#define PI 3.1415926535897932384626433832795

uniform float _time;
uniform vec2 _origin;
uniform vec2 _window_resolution;

float atan01(float x, float y)
{
	return (atan(x, y) / PI + 1) / 2;
}
float inverse_mix(float a, float b, float t){
	float t_clamped = clamp(t, a, b);
	return (t_clamped-a)/(b-a);
}

float soft_threshold(float t, float threshold, float softness){
	return mix(0, 1, inverse_mix(threshold-0.5*softness, threshold+0.5*softness, t));
}
void main()
{
	vec2 screen_coords = gl_FragCoord.xy;
	screen_coords.y = _window_resolution.y - screen_coords.y;
	vec2 local_coords = screen_coords - _origin;
	local_coords /= 0.4*_window_resolution.y;
	float r = length(local_coords) + 0.2*sin(0.2*2*PI*_time);
	float theta01 = atan01(local_coords.x, local_coords.y);

	gl_FragColor = vec4(theta01, r, 0, 0.5*(1-soft_threshold(r, 1, 0.1))*soft_threshold(fract(5*(theta01+0.2*_time)), 0.5, 0.2));
	return;
}