#version 120

#include "shaders\\include\\standard_uniforms.glsl";
#include "shaders\\include\\math_utils.glsl";
#include "shaders\\include\\game_constants.glsl";

uniform int n_dots;
uniform int active_dot;

#define R 15

#define DEFAULT_COLOR vec3(float(100)/float(256))
#define ACTIVE_COLOR vec3(float(200)/float(256))

void main()
{
    vec2 c = gl_TexCoord[0].xy*_wh;
    float cell_w = _wh.x/n_dots;
    float id = int(c.x/cell_w);
    vec2 gc = vec2(fract(c.x/cell_w), c.y/cell_w);
    float d = cell_w*length(gc-0.5*vec2(1, _wh.y/cell_w));
    float sd = d-R;

    vec3 color = mix(DEFAULT_COLOR, ACTIVE_COLOR, float(abs(id-active_dot) < 0.001));

    gl_FragColor = vec4(color, smoothstep(GAME_SPACE_AA, 0, sd));
    //gl_FragColor = vec4(d/60, 0, 0, 1);
}