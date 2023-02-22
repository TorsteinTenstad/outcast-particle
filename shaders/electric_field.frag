#version 120

uniform float _time;
uniform vec2 _wh;

float rand(float seed){ 
    return fract(sin(dot(vec2(seed, fract(seed*0.341)), vec2(12.9898, 78.233))) * 43758.5453);
}

#define R 20

void main()
{
   	vec2 c = (gl_TexCoord[0].xy*120)/vec2(R);
    float gc_id = floor(c.x);
    vec2 gc = vec2(fract(c.x), c.y);
    float period = rand(gc_id);
    float existance = rand(period);
    float y = (200*_time/_wh.y*(0.8+0.2*rand(existance)) + period);
    y = fract(y);
    y = y*y*y;

    y = y*(_wh.y/R+1.f)-0.5;
    vec2 d = gc - vec2(0.5, y);
    float sd = length(d)-0.5;
    float ball = smoothstep(0.0, -0.05, sd);
    vec4 col = vec4(0);
	col += 0.7*ball*float(existance>0.5)*vec4(1);
    col += mix(vec4(0), vec4(0, 1, 0, 1), smoothstep(0.3, 1, 1-gl_TexCoord[0].y*120/_wh.y))*0.2;
    col += mix(vec4(0), vec4(1, 0, 0, 1), smoothstep(0.3, 1, gl_TexCoord[0].y*120/_wh.y))*0.2;
	gl_FragColor = col;
}