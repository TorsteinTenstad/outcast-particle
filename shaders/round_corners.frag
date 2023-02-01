#version 120
#define PI 3.14193
#define r 50

uniform sampler2D texture;

uniform vec2 _wh;

void main()
{
	// lookup the pixel in the texture
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    vec2 folded_coords = abs(gl_TexCoord[0].xy-(_wh)/2);
    vec2 r_smaller_box = min(folded_coords, vec2(_wh)/2-r);
    vec2 dist_from_smaller_box = folded_coords-r_smaller_box;
    float alpha = 1-smoothstep(r-2, r+2, length(dist_from_smaller_box));
	// multiply it by the color
	gl_FragColor = gl_Color * pixel*vec4(vec3(1), alpha);
}