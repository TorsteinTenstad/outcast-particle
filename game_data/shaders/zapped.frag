#version 120

#include "game_data\\shaders\\include\\standard_uniforms.glsl";
#include "game_data\\shaders\\include\\math_utils.glsl";

uniform float start_animation = -1.;

void main()
{
	// lookup the pixel in the texture
	vec4 pixel = texture2D(_texture, gl_TexCoord[0].xy);
	float t = (_time - start_animation);
	if (0 < start_animation)
	{
		float animation_step = fract(2.5*t);
		if (animation_step < 0.7){
			pixel.rgb = vec3(0, 0, 0);
		}
	}
	// multiply it by the color
	gl_FragColor = gl_Color * pixel;
}