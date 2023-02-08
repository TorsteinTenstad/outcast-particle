#version 120

#define PI 3.1415926535897932384626433832795


uniform float _time;
uniform vec2 origin;
uniform vec2 _window_resolution;
uniform vec2 _wh;

float radial_falloff(float r, float r1, float r0){
	float f = 1/(max((r-r1), 0)/r0+1);
	return smoothstep(0.5, 1, f);
}


#define color vec3(1, 0.9, 0.27)
#define n 5
#define half_intensity_r 180
#define rotational_frequency 0.5
#define radial_frequency 0.5

void main()
{
	vec2 screen_coords = gl_TexCoord[0].xy;
	vec2 local_coords = screen_coords - origin;
	float r = length(local_coords);
    float theta = atan(local_coords.x, local_coords.y);
    float a = 0.5*theta/PI+0.5;

	float intensity = radial_falloff(r, 0, half_intensity_r+15*sin(2*PI*radial_frequency*_time));
	float radial_mask = float(fract(n*a+rotational_frequency*_time) > 0.5);
	float alpha = intensity*radial_mask;

	gl_FragColor = vec4(color, alpha);
	return;
}