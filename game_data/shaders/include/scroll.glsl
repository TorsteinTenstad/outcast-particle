#include "shaders\\include\\game_constants.glsl";
#include "shaders\\include\\standard_uniforms.glsl";

uniform float top = -100000000;
uniform float bottom = 100000000;

float get_scroll_alpha()
{
    vec2 gs_c = _view_size*gl_FragCoord.xy/_window_resolution + (_view_size-_level_size)/2;
    gs_c.y = _view_size.y - gs_c.y;
    float upper_fade_out = smoothstep(top-GAME_SPACE_AA, top, gs_c.y);
    float lower_fade_out = 1-smoothstep(bottom, bottom+GAME_SPACE_AA, gs_c.y);

    float alpha = 1;
	alpha*=upper_fade_out;
	alpha*=lower_fade_out;
    return alpha;
}