#version 120

uniform float _time = 0.;
uniform float start_switch_charge_animation = -1.;
uniform float start_death_animation = -1.;
#define PI 3.1415926535897932384626433832795


void main()
{
    // transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    vec2 centered_coord = 2*(gl_TexCoord[0].xy-0.5);

    vec2 offset = vec2(0, 0);

	float switch_charge_t = (_time - start_switch_charge_animation)*8;
	if (0 < start_switch_charge_animation && 0 < switch_charge_t && switch_charge_t < 1)
	{
        offset -= 20*centered_coord*sin(PI*switch_charge_t);
	}
    
	float death_t = (_time - start_death_animation);
    if (0 < start_death_animation && 0 < death_t)
	{
        float theta = atan(centered_coord.x, centered_coord.y);
        offset += centered_coord*10*sin(2*2*PI*theta+30*death_t);
	}

    // transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_Vertex.xy+offset, gl_Vertex.zw);

    // forward the vertex color
    gl_FrontColor = gl_Color;
}