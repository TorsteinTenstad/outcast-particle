#version 120

#include "shaders\\include\\standard_uniforms.glsl";
#include "shaders\\include\\math_utils.glsl";
#include "shaders\\include\\game_constants.glsl";
#include "shaders\\include\\blend.glsl";
#include "shaders\\include\\gem.glsl";

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


    float r = _wh.y/4;
	vec2 folded_coords = abs(xy-(_wh)/2);
    vec2 r_smaller_box = min(folded_coords, vec2(_wh)/2-r);
    vec2 dist_from_smaller_box = folded_coords-r_smaller_box;
    float round_corner_mask = 1-smoothstep(r-2, r+2, length(dist_from_smaller_box));

    vec4 color = vec4(0.4, 0.4, 0.4, 1);

	color = blend(color, gem((c_uv-vec2(1*margin+1*gem_r,0))/gem_size, GAME_SPACE_AA/gem_size)*vec4(vec3(1), clamp(float(n_collected)-0.f, 0.20f, 1.f)));
	color = blend(color, gem((c_uv-vec2(2*margin+3*gem_r,0))/gem_size, GAME_SPACE_AA/gem_size)*vec4(vec3(1), clamp(float(n_collected)-1.f, 0.20f, 1.f)));
	color = blend(color, gem((c_uv-vec2(3*margin+5*gem_r,0))/gem_size, GAME_SPACE_AA/gem_size)*vec4(vec3(1), clamp(float(n_collected)-2.f, 0.20f, 1.f)));

	gl_FragColor = color * round_corner_mask * gl_Color;
}