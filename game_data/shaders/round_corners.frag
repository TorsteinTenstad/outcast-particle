#version 120

#include "game_data\\shaders\\include\\standard_uniforms.glsl";
#include "game_data\\shaders\\include\\round_corners.glsl";

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec2 xy = uv*_wh;
	// lookup the pixel in the texture
	vec4 pixel = texture2D(_texture, gl_TexCoord[0].xy);

    float alpha = get_rounded_corners_alpha(xy, _wh);

    // multiply it by the color
	gl_FragColor = gl_Color * pixel * vec4(vec3(1), alpha);
}