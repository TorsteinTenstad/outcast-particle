#version 120

uniform float _time = 0.;
uniform vec2 _wh;
uniform float start_switch_charge_animation = -1.;
uniform float time_of_last_level_enter = -1.;
#define PI 3.1415926535897932384626433832795

float ease(float t, float undershoot, float overshoot){
    t = clamp(t, 0, 1);
    float a =    overshoot*pow(t  , 2);
    float b = 1-undershoot*pow(t-1, 2);
    return mix(a, b, smoothstep(0, 1, t));
}

float ease(float t){
    return ease(t, 2.0, 4.0);
}

void main()
{
    // transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    vec2 centered_coord = 2*(gl_TexCoord[0].xy/_wh-0.5);

    vec2 offset = vec2(0, 0);

	float switch_charge_t = (_time - start_switch_charge_animation)*8;
	float enter_level_t = (_time - time_of_last_level_enter)*3;
	if (0 < start_switch_charge_animation && 0 < switch_charge_t && switch_charge_t < 1)
	{
        offset -= 0.1*centered_coord*sin(PI*switch_charge_t);
	}
    else if (0 < enter_level_t && enter_level_t < 1.5)
	{
        offset -= 0.5*centered_coord*(1-ease(enter_level_t-0.5));
	}

    // transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_Vertex.xy+offset*_wh, gl_Vertex.zw);

    // forward the vertex color
    gl_FrontColor = gl_Color;
}