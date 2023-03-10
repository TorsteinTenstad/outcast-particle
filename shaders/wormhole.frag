#version 120

#define PI 3.1415926535897932384626433832795

uniform vec2 _wh;
uniform float _time;
#define BLOCK_SIZE 120

float tanh(float x){
    return (exp(x)-exp(-x))/(exp(x)+exp(-x));
}

float max_component(vec2 v){
    return max(v.x, v.y);
}

float sin01(float x){
    return 0.5*sin(2*PI*x)+0.5;
}

vec4 blend(vec4 base, vec4 top){
	float a = top.a + base.a * (1-top.a);
	if (abs(a) < 0.0001){
		return vec4(0);
	}
	vec3 rgb = (top.a*top.rgb+base.a*base.rgb*(1-top.a))/a;
	return vec4(rgb, a);
}



void main()
{
    float R = 2;

	vec2 uv = gl_TexCoord[0].xy;
	vec2 c_uv = (uv - _wh/2)/BLOCK_SIZE;
	float r = length(c_uv);
    float theta = atan(c_uv.x, c_uv.y);
    float a = 0.5*theta/PI+0.5;

    //c_uv = max(0, 0.4*(3*log(r)+r)+1)*c_uv/length(c_uv);
    float f = sqrt(sqrt(pow(r/2+1, 4)-1));
    c_uv = (f)*c_uv/length(c_uv);
    float r_tranformed = length(c_uv);


    float AA = 10.f/float(BLOCK_SIZE);
    float g = smoothstep(0, AA, max_component(abs(fract(3*c_uv)-0.5))-0.43);
    float light = exp(1.5*r+1-R)/exp(1.f);
    //light *= smoothstep(0.35, 0.5, r);
    //light = 1;
    float swirl = sin01(r_tranformed + a + 0.2*_time + 0.06*sin(10*theta));
    //vec3 swirl_color = mix(vec3(0.34, 0.1, 1), mix(vec3(0.45, 0.5, 1), vec3(0.4, 0.5, 0.8), sin01(a+0.01*_time)), swirl);
    vec3 swirl_color = mix(vec3(0.4, 0.1, 1), mix(vec3(0.5, 0.5, 1), vec3(0.3, 0.7, 1), sin01(a+0.01*_time)), swirl);
    //vec3 swirl_color = mix(vec3(0.34, 0.1, 0.8), mix(vec3(0.55, 0.5, 0.8), vec3(0.4, 0.6, 0.8), sin01(a+0.01*_time)), swirl);
    
    vec4 forground = blend(vec4(swirl_color, 1), vec4(vec3(0.8), g*0.5));
    forground.a = light;
    vec4 color = vec4(0,0,0,1);
    color = blend(color, forground);



    gl_FragColor = color*(1-smoothstep(-1.2, 0, r-0.3-R-0.1*sin01(10*a+2*r)));
}