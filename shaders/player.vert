
uniform float _time;
uniform float start_animation;
#define PI 3.1415926535897932384626433832795


void main()
{
	float t = (_time - start_animation)*8;
	if (t > 1 || t < 0)
	{
		t = 0;
	}

    // transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    vec2 offset = 20*2*(gl_TexCoord[0].xy-0.5)*sin(PI*t);
    gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_Vertex.xy-offset, gl_Vertex.zw);
    // forward the vertex color
    gl_FrontColor = gl_Color;
}