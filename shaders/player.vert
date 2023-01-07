
uniform float _time;
uniform vec2 _position;
uniform float start_animation;
#define PI 3.1415926535897932384626433832795


void main()
{
	float t = (_time - start_animation) / 8;
	if (t > 1 || t < 0)
	{
		t = 0;
	}
    //gl_Vertex.xy -= (gl_Vertex-_position)*0.5*sin(PI*t);
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    // forward the vertex color
    gl_FrontColor = gl_Color;
    //gl_FrontColor = vec4(gl_Position.xy, 0, 1);
}