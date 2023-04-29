#version 120

uniform float _time = 0.;
uniform vec2 _wh;
uniform float start_animation = -1.;
#define PI 3.1415926535897932384626433832795


void main()
{
    // transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    vec2 centered_coord = 2*(gl_TexCoord[0].xy-0.5);
    vec2 offset = vec2(0, 0);
    
	float t = (_time - start_animation);
    if (0 < start_animation && 0 < t)
	{
        float theta = atan(centered_coord.x, centered_coord.y);
        offset += centered_coord*10*sin(2*2*PI*theta+30*t);
	}

    vec4 color_filter = vec4(1);
	if (0 < start_animation)
	{
		float animation_step = fract(1.5*t);
		if (animation_step < 0.7){
			color_filter.rgb = vec3(0, 0, 0);
		}
	}

    // transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_Vertex.xy+offset, gl_Vertex.zw);

    // forward the vertex color
    gl_FrontColor = gl_Color*color_filter;
}