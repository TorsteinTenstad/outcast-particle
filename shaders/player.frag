#version 120

uniform sampler2D texture;
uniform float _time = 0.;
uniform float start_death_animation = -1.;
#define PI 3.1415926535897932384626433832795

void main()
{
	// lookup the pixel in the texture
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	float t = (_time - start_death_animation);
	if (0 < start_death_animation)
	{
		float animation_step = fract(2.5*t);
		if (animation_step < 0.7){
			pixel.rgb = vec3(0, 0, 0);
		}
	}
	// multiply it by the color
	gl_FragColor = gl_Color * pixel;
}