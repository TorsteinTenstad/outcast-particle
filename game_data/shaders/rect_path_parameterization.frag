#version 120

#include "game_data\\shaders\\include\\standard_uniforms.glsl";
#include "game_data\\shaders\\include\\game_constants.glsl";
#include "game_data\\shaders\\include\\math_utils.glsl";

void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec2 xy = uv*_wh;

    vec2 local_wh = vec2(
        2.0*abs(uv.y-0.5)*_wh.x,
        2.0*abs(uv.x-0.5)*_wh.y
    );
    vec2 c_xy = xy - _wh*0.5;
    vec2 remapped_xy = (c_xy/local_wh + 0.5);
    bool a = uv.x < uv.y;
    bool b = uv.x > (1.0 - uv.y);
    float t = (
          (                        _wh.x *      remapped_xy.x ) * float(!a && !b) 
        + (_wh.x +                 _wh.y *      remapped_xy.y ) * float(!a &&  b) 
        + (_wh.x + _wh.y +         _wh.x * (1.0-remapped_xy.x)) * float( a &&  b)
        + (_wh.x + _wh.y + _wh.x + _wh.y * (1.0-remapped_xy.y)) * float( a && !b)
        ) / (2.0*_wh.x + 2.0*_wh.y); 
    vec4 color = vec4(vec3(sin01(fract(5*t)+_time)), 1.0);
    float mask = smoothstep(0, GAME_SPACE_AA, max_component(abs(xy - _wh*0.5) - _wh*0.5 + BLOCK_SIZE/6));
	// lookup the pixel in the texture
	vec4 pixel = texture2D(_texture, gl_TexCoord[0].xy);
    // multiply it by the color
	gl_FragColor = mix(gl_Color * pixel, color, mask);
}