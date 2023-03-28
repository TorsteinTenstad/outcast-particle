#version 120

#define PI 3.1415926535897932384626433832795
#define SQRT2 1.41421356237

uniform float _time;
uniform float animation_start_time = -1;
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

float ease(float t, float undershoot, float overshoot){
    t = clamp(t, 0, 1);
    float a =    overshoot*pow(t  , 2);
    float b = 1-undershoot*pow(t-1, 2);
    return mix(a, b, smoothstep(0, 1, t));
}

float ease(float t){
    return ease(t, 1, 1);
}

#define n 5
#define glow_r 180
#define rotational_base_frequency 0.5
#define radial_frequency 0.5
#define AA 3
#define glow_color vec3(1, 0.9, 0.27)


vec4 gem(vec2 uv){
	vec2 gem_c = abs(rot(PI/4)*uv);
	vec2 gem_color_c = (1.f/120.f)*(rot(PI/8)*(uv+vec2(0, 60)));
	//vec3 gem_color = gem_color_c.x*vec3(0.8, 0.8, 0.1) + gem_color_c.y*vec3(0.9, 0.9, 0.9);
	vec3 gem_color = gem_color_c.x*vec3(0, 1, 1) + gem_color_c.y*vec3(0, 0, 1);
	float gem_alpha = 1-smoothstep(0, AA, max(gem_c.x, gem_c.y)-SQRT2*30);
	vec4 gem_contribution = vec4(1-gem_color, gem_alpha);
	return gem_contribution;
}

void main()
{
	float animation_t = animation_start_time > 0 ? _time - animation_start_time : 0;

	vec2 screen_coords = gl_TexCoord[0].xy;
	vec2 uv = screen_coords - origin - vec2(0, -300*animation_t);
	float r = length(uv);
    float theta = atan(uv.x, uv.y);
    float a = 0.5*theta/PI+0.5;

	float intensity = radial_falloff(r, 0, glow_r+15*sin(2*PI*radial_frequency*_time));
	float radial_mask = float(fract(n*a+rotational_base_frequency*_time+1.5*ease(animation_t)) > 0.5);
	float glow_alpha = intensity*radial_mask;
	vec4 glow_contribution = vec4(glow_color, glow_alpha);

	vec4 gem_contribution = gem(uv);

	vec4 color = blend(glow_contribution, gem_contribution);
	color.a *= 1-animation_t;
	gl_FragColor = color;
	//gl_FragColor = gem_contribution;
}