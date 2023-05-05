#version 120

#include "shaders\\include\\standard_uniforms.glsl";
#include "shaders\\include\\game_constants.glsl";
#include "shaders\\include\\math_utils.glsl";

float arrows(vec2 uv, float AA, float n){
    float y_margin = 0.1;
    float x_margin = 0.1;
    float spacing = 0.1;
    float slope = 0.8;

    float thickness = 0.2;

    float c = uv.y-slope*abs(uv.x-0.5) - y_margin;
    float repeat_period = thickness + spacing;
    float id = get_repeat_id(c, repeat_period);
    float repeated_c = repeat(c, repeat_period);
    float id_mask = step(-0.5, id) * (1-step(n-0.5, id));
	float front_mask = smoothstep(0, AA, repeated_c);
	float back_mask = 1 - smoothstep(-AA, 0, repeated_c - thickness);
	float width_mask = 1 - smoothstep(-AA, 0, abs(uv.x-0.5) - (0.5-x_margin));
    float arrow = front_mask*back_mask*width_mask*id_mask;
    return arrow;
}


vec4 strength_indicator(vec2 uv, float AA, float categorical_value){
    float signed_n = categorical_value-2;
    float n = abs(signed_n);
    float rotate = 0.5+0.5*sign(-signed_n);
    uv = rot(PI*rotate)*(uv-0.5)+0.5;

    float arrow = arrows(uv, AA, n);
    float gradient = (uv.x + uv.y)/2;
    gradient = 0.5 + 0.5 * gradient;
    return vec4(vec3(1), arrow);
}

uniform float category_idx = 2;


void main()
{
    vec2 uv = gl_TexCoord[0].xy;
    vec2 xy = uv*_wh;
    
    vec4 color = strength_indicator(uv, GAME_SPACE_AA/BLOCK_SIZE, category_idx  );

	gl_FragColor = gl_Color * color;
}