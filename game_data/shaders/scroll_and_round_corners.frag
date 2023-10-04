#version 120

#include "game_data\\shaders\\include\\standard_uniforms.glsl";
#include "game_data\\shaders\\include\\scroll.glsl";
#include "game_data\\shaders\\include\\round_corners.glsl";
#include "game_data\\shaders\\include\\game_constants.glsl";

uniform vec4 border_color = vec4(1); 

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec2 xy = uv*_wh;
    
	// lookup the pixel in the texture
	vec4 pixel = texture2D(_texture, gl_TexCoord[0].xy);

    float boarder_mask = 1-get_rounded_corners_mask(xy-BLOCK_SIZE/12, _wh-BLOCK_SIZE/6, min(_wh.x, _wh.y)/4);
    vec4 color = mix(vec4(1), border_color, boarder_mask);
    color.a *= get_rounded_corners_alpha(xy, _wh);
    color.a *= get_scroll_alpha();

    // multiply it by the color
	gl_FragColor = gl_Color * pixel * color;
}