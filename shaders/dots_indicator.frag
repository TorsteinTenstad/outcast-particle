#version 120

#define PI 3.14193

uniform float _time;
uniform vec2 _wh;

uniform int n_dots;
uniform int active_dot;

#define R 15
#define GAME_SPACE_AA 3

#define DEFAULT_COLOR vec3(float(100)/float(256))
#define ACTIVE_COLOR vec3(float(200)/float(256))

void main()
{
    vec2 c = gl_TexCoord[0].xy*_wh;
    float cell_w = _wh.x/n_dots;
    float id = int(c.x/cell_w);
    vec2 gc = fract(c/cell_w);
    float d = cell_w*length(gc-0.5*vec2(1, _wh.y/cell_w));
    float sd = d-R;

    vec3 color = mix(DEFAULT_COLOR, ACTIVE_COLOR, float(abs(id-active_dot) < 0.001));

    gl_FragColor = vec4(color, smoothstep(GAME_SPACE_AA, 0, sd));
}