#version 120

uniform float _time = 0.;
uniform float start_switch_charge_animation = -1.;
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

    // transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_Vertex.xy+offset, gl_Vertex.zw);

    // forward the vertex color
    gl_FrontColor = gl_Color;
}