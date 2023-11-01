#version 120

#include "game_data\\shaders\\include\\standard_uniforms.glsl";
#include "game_data\\shaders\\include\\game_constants.glsl";
#include "game_data\\shaders\\include\\math_utils.glsl";

uniform float animation_start_time = 1.0;

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec2 xy = uv*_wh;
    float animation_duration = 0.8;
    float animation_t = (_time - animation_start_time)/animation_duration;
    animation_t *= float(animation_t < 1);

    float r = length(xy - _wh*0.5)/length(_wh*0.5);
    float w = 1-cos01(2*animation_t);
	// lookup the pixel in the texture
	vec4 pixel = texture2D(_texture, gl_TexCoord[0].xy);
    // multiply it by the color
	gl_FragColor = gl_Color * pixel * (1+0.5*w);
}