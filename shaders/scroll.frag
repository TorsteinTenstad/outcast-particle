#version 120
#define PI 3.14193
#define r 50

uniform sampler2D texture;

uniform vec2 _wh;
uniform vec2 _window_resolution;
uniform vec2 _view_size;
uniform vec2 _view_center;
uniform vec2 _level_size;
uniform float top = -100000000;
uniform float bottom = 100000000;
#define GS_AA 3

void main()
{
	// lookup the pixel in the texture
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    vec2 gs_c = _view_size*gl_FragCoord.xy/_window_resolution + (_view_size-_level_size)/2;
    gs_c.y = _view_size.y - gs_c.y;
    float upper_fade_out = smoothstep(top-GS_AA, top, gs_c.y);
    float lower_fade_out = 1-smoothstep(bottom, bottom+GS_AA, gs_c.y);

    float alpha = 1;
	alpha*=upper_fade_out;
	alpha*=lower_fade_out;
    // multiply it by the color
	gl_FragColor = gl_Color * pixel* vec4(vec3(1), alpha);
}