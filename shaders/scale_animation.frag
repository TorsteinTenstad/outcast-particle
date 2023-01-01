#version 120
uniform sampler2D currentTexture;
uniform float _time;
uniform float start_animation;
#define PI 3.1415926535897932384626433832795
void main()
{
	vec2 coord = gl_TexCoord[0].xy;
	float t = (_time - start_animation) * 8;
	if (t > 1 || t < 0)
	{
		t = 0;
	}
	coord.x -= (0.5 - coord.x) * sin(PI * t) * 0.2;
	coord.y -= (0.5 - coord.y) * sin(PI * t) * 0.2;
	vec4 pixel_color = texture2D(currentTexture, coord);
	if (coord.x < 0.0001 || coord.x > 0.9999 || coord.y < 0.0001 || coord.y > 0.9999)
	{
		gl_FragColor = vec4(0, 0, 0, 0);
	}
	else
	{
		float amplitdue = 0.1;
		gl_FragColor = pixel_color + vec4(vec3(amplitdue * sin(PI * t)), 0);
	}
}
