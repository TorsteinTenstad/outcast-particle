#version 120
#define PI 3.14193

uniform sampler2D texture;

uniform vec2 _wh;
uniform vec2 _window_resolution;
uniform vec2 _view_size;
uniform vec2 _view_center;
uniform vec2 _level_size;
#define GS_AA 3

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec2 xy = uv*_wh;
	// lookup the pixel in the texture
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    vec2 gs_c = _view_size*gl_FragCoord.xy/_window_resolution + (_view_size-_level_size)/2;
    gs_c.y = _view_size.y - gs_c.y;
    float r = min(_wh.x, _wh.y)/4;

    vec2 folded_coords = abs(xy-(_wh)/2);
    vec2 r_smaller_box = min(folded_coords, vec2(_wh)/2-r);
    vec2 dist_from_smaller_box = folded_coords-r_smaller_box;
    float alpha = 1-smoothstep(r-2, r+2, length(dist_from_smaller_box));
    // multiply it by the color
	gl_FragColor = gl_Color * pixel*vec4(vec3(1), alpha);
    //gl_FragColor = vec4(gl_TexCoord[0].xy, 0, 1);
    //gl_FragColor = vec4(gs_c.y-top,0, 0, 1);
}