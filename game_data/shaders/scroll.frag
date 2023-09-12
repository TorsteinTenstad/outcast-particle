#version 120

#include "game_data\\shaders\\include\\scroll.glsl";

void main()
{
	// lookup the pixel in the texture
	vec4 pixel = texture2D(_texture, gl_TexCoord[0].xy);

    float alpha = get_scroll_alpha();

    // multiply it by the color
	gl_FragColor = gl_Color * pixel * vec4(vec3(1), alpha);
}