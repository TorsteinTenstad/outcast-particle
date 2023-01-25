#version 120

#define PI 3.1415926535897932384626433832795


uniform float _time;
uniform vec2 _origin;
uniform vec2 _window_resolution;
uniform vec2 _wh;

float atan01(float x, float y)
{
	return (atan(x, y) / PI + 1) / 2;
}

float sin01(float x){
	return 0.5+0.5*sin(x);
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
	local_coords /= mix(0.06, 0.08, sin01(2*_time))*_window_resolution.y;
	float r = length(local_coords);
	float theta = atan01(local_coords.x, local_coords.y);

	gl_FragColor = vec4(mix(vec3(1, 0.9, 0.27), vec3(1), r), 0.5*max(0, 1-r)*float(fract(5*theta+_time)>0.5));
	return;
}