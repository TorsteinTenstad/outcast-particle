#version 120

#include "game_data\\shaders\\include\\standard_uniforms.glsl";
#include "game_data\\shaders\\include\\math_utils.glsl";
#include "game_data\\shaders\\include\\game_constants.glsl";
#include "game_data\\shaders\\include\\blend.glsl";
#include "game_data\\shaders\\include\\gem.glsl";

uniform float animation_start_time = -1;
uniform vec2 origin;

float ease(float t){
    return ease(t, 1, 1);
}

#define n 5
#define glow_r 180
#define rotational_base_frequency 0.5
#define radial_frequency 0.5
#define AA 3
#define glow_color vec3(1, 0.9, 0.27)

void main()
{
	float animation_t = animation_start_time > 0 ? _time - animation_start_time : 0;

	vec2 xy = gl_TexCoord[0].xy*_wh;
	vec2 centered_xy = xy - origin - vec2(0, -300*animation_t);
	float r = length(centered_xy);
    float theta = atan(centered_xy.x, centered_xy.y);
    float a = 0.5*theta/PI+0.5;

	float intensity = radial_falloff(r, 0, glow_r+15*sin(2*PI*radial_frequency*_time));
	float radial_mask = float(fract(n*a+rotational_base_frequency*_time+1.5*ease(animation_t)) > 0.5);
	float glow_alpha = intensity*radial_mask;
	vec4 glow_contribution = vec4(glow_color, glow_alpha);

	vec4 gem_contribution = gem(centered_xy/float(BLOCK_SIZE), GAME_SPACE_AA/float(BLOCK_SIZE));

	vec4 color = blend(glow_contribution, gem_contribution);
	color.a *= 1-animation_t;
	gl_FragColor = color;
	//gl_FragColor = vec4(centered_xy, 0, 1);
}