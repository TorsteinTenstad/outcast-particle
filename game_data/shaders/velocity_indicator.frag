#version 120

#include "shaders\\include\\standard_uniforms.glsl";
#include "shaders\\include\\math_utils.glsl";
#include "shaders\\include\\game_constants.glsl";

uniform float magnitude;
uniform float angle;

void main()
{
	vec2 uv = gl_TexCoord[0].xy;
    vec2 xy = uv*_wh;
    float radius = _wh.x/2;
    uv = rot(angle)*(uv-0.5)*radius;
    float r = 1;
    float aa = GAME_SPACE_AA/radius;
    r *= 1-smoothstep(0, GAME_SPACE_AA, abs(uv.y)-BLOCK_SIZE/8);
    r *= smoothstep(0, GAME_SPACE_AA, uv.x);

    
    gl_FragColor = gl_Color*vec4(1, 1, 1, r);
}