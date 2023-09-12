#version 120

#include "shaders\\include\\standard_uniforms.glsl";
#include "shaders\\include\\math_utils.glsl";
#include "shaders\\include\\game_constants.glsl";

#define BORDER_RGB vec3(0.86274509803)
#define BACKGROUND_RGB vec3(0.2)
#define WIDTH 4

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec2 xy = uv*_wh;

    float val = smoothstep(0, GAME_SPACE_AA, min(min_component(xy), min_component(_wh-xy)) -WIDTH);

    vec3 color = mix(BORDER_RGB, BACKGROUND_RGB, val);
	gl_FragColor = gl_Color * vec4(color, 1);
}