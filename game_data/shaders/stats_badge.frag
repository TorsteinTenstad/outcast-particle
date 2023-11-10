#version 120

#include "game_data\\shaders\\include\\standard_uniforms.glsl";
#include "game_data\\shaders\\include\\math_utils.glsl";
#include "game_data\\shaders\\include\\game_constants.glsl";
#include "game_data\\shaders\\include\\blend.glsl";
#include "game_data\\shaders\\include\\gem.glsl";
#include "game_data\\shaders\\include\\rand.glsl";
#include "game_data\\shaders\\include\\round_corners.glsl";

float coin_size = 120;
float coin_spacing = 60;

uniform int n_collected = 2;

void main()
{
	vec2 uv = gl_TexCoord[0].xy;
	vec2 xy = uv*_wh;
    vec2 c_uv = xy-vec2(0, _wh.y/2);

	float margin_ratio = 0.25;
	float gem_size = _wh.y/(1.f+2.f*margin_ratio);
	float margin = gem_size*margin_ratio;
	float gem_r = gem_size/2.f;

    float round_corner_mask = get_rounded_corners_alpha(xy, _wh);

    vec4 color = gl_Color;

	color = blend(color, gem((c_uv-vec2(1*margin+1*gem_r,0))/gem_size, GAME_SPACE_AA/gem_size)*vec4(vec3(1), clamp(float(n_collected)-0.f, 0.20f, 1.f)));
	color = blend(color, gem((c_uv-vec2(2*margin+3*gem_r,0))/gem_size, GAME_SPACE_AA/gem_size)*vec4(vec3(1), clamp(float(n_collected)-1.f, 0.20f, 1.f)));
	color = blend(color, gem((c_uv-vec2(3*margin+5*gem_r,0))/gem_size, GAME_SPACE_AA/gem_size)*vec4(vec3(1), clamp(float(n_collected)-2.f, 0.20f, 1.f)));

	gl_FragColor = color * round_corner_mask;
}