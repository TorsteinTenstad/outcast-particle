
uniform float _time;
#define PI 3.1415926535897932384626433832795

float offset(vec2 co, float time, float base_phase){
	float omega = 40;
	float phase = dot(co, vec2(1, 0))+dot(co, vec2(0, 1));
    return (sin((omega+base_phase)*time+phase + base_phase));
}

void main()
{
    // transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position.x += offset(gl_Vertex.xy, _time, 1)*0.001;
	gl_Position.y += offset(gl_Vertex.xy, _time, 2)*0.001;

    // transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    // forward the vertex color
    gl_FrontColor = vec4((0.4*offset(gl_Vertex.xy, _time, 3))+gl_Color.rgb, 1);
}