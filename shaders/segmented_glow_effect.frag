#version 120

#define PI 3.1415926535897932384626433832795
#define SQRT2 1.41421356237

uniform float _time;
uniform vec2 origin;
uniform vec2 _window_resolution;
uniform vec2 _wh;

float radial_falloff(float r, float a, float b){
	float f = 1/(max((r-a), 0)/(b-a)+1);
	return smoothstep(0.5, 1, f);
}

vec4 blend(vec4 base, vec4 top){
	float a = top.a + base.a * (1-top.a);
	if (abs(a) < 0.0001){
		return vec4(0);
	}
	vec3 rgb = (top.a*top.rgb+base.a*base.rgb*(1-top.a))/a;
	return vec4(rgb, a);
}

mat2 rot(float theta) {
	float s = sin(theta);
	float c = cos(theta);
	mat2 m = mat2(c, -s, s, c);
    return m;
}


#define glow_color vec3(1, 0.9, 0.27)
//#define gem_color vec3(1, 1, 1)
#define n 5
#define glow_r 180
#define rotational_frequency 0.5
#define radial_frequency 0.5
#define AA 3

void main()
{
	vec2 screen_coords = gl_TexCoord[0].xy;
	vec2 lc = screen_coords - origin;
	float r = length(lc);
    float theta = atan(lc.x, lc.y);
    float a = 0.5*theta/PI+0.5;

	float intensity = radial_falloff(r, 0, glow_r+15*sin(2*PI*radial_frequency*_time));
	float radial_mask = float(fract(n*a+rotational_frequency*_time) > 0.5);
	float glow_alpha = intensity*radial_mask;
	vec4 glow_contribution = vec4(glow_color, glow_alpha);

	vec2 gem_c = abs(rot(PI/4)*lc);
	vec2 gem_color_c = (1.f/120.f)*abs(rot(PI/8)*(lc+vec2(0, 60)));
	//vec3 gem_color = gem_color_c.x*vec3(0.8, 0.8, 0.1) + gem_color_c.y*vec3(0.9, 0.9, 0.9);
	vec3 gem_color = gem_color_c.x*vec3(0, 1, 1) + gem_color_c.y*vec3(0, 0, 1);
	float gem_alpha = 1-smoothstep(0, AA, max(gem_c.x, gem_c.y)-SQRT2*30);
	vec4 gem_contribution = vec4(1-gem_color, gem_alpha);

	gl_FragColor = blend(glow_contribution, gem_contribution);
	//gl_FragColor = gem_contribution;
}