#version 120

#define PI 3.1415926535897932384626433832795

uniform float _time;
uniform vec2 _wh;

float rand(float seed){ 
    return fract(sin(dot(vec2(seed, fract(seed*0.341)), vec2(12.9898, 78.233))) * 43758.5453);
}
#define DISC_CENTER 0.6
#define DISC_THICKNESS 0.15
void main()
{
   	vec2 c = gl_TexCoord[0].xy;
    vec2 gc_id = floor(c);
    vec2 gc = fract(c);
    vec2 p = gc - vec2(0.5, 0.5);
    float r = 2*length(p);
    float theta = atan(p.x, p.y);
    float a = 0.5*theta/PI+0.5;
    a = fract(a+0.1*_time);

    float inner_mask = smoothstep(0, 0.02, r-DISC_CENTER+DISC_THICKNESS);
    float outer_mask = smoothstep(0, 0.02, DISC_CENTER+DISC_THICKNESS-r);

    float arrow_mask = smoothstep(0, 0.02, a-1.0/6.0);
    
	gl_FragColor = vec4(vec3(1), 0.5*inner_mask*outer_mask*arrow_mask);
}