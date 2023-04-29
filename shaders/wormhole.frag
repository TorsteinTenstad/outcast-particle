#version 120

#define PI 3.1415926535897932384626433832795

uniform vec2 _wh;
uniform float _time;
uniform sampler2D _noise_texture;
#define noise_size 1000
#define BLOCK_SIZE 120

float N21(vec2 uv){
    uv = fract(uv/noise_size);
    return texture2D(_noise_texture, uv).r;
}

float SmoothNoise(vec2 uv) {
    vec2 lv = fract(uv);
    vec2 id = floor(uv);
    
    lv = lv*lv*(3.-2.*lv);
    
    float bl = N21(id);
    float br = N21(id+vec2(1,0));
    float b = mix(bl, br, lv.x);
    
    float tl = N21(id+vec2(0,1));
    float tr = N21(id+vec2(1,1));
    float t = mix(tl, tr, lv.x);
    
    return mix(b, t, lv.y);
}

float LayeredSmoothNoise(vec2 uv) {
    float c = 0;
    c += SmoothNoise(uv* 2.0)*1.0;
    c += SmoothNoise(uv* 4.2)*0.5;
    c += SmoothNoise(uv* 8.7)*0.25;
    c += SmoothNoise(uv*16.4)*0.125;
    c += SmoothNoise(uv*32.5)*0.0625;
    c /= 2.;
    return c;
}

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
	vec2 uv = gl_TexCoord[0].xy;
    vec2 xy = uv*_wh;


    float R = 2;

	vec2 c_uv = (xy - _wh/2)/(BLOCK_SIZE);
	float r = length(c_uv);
    float theta = atan(c_uv.x, c_uv.y);
    float a = 0.5*theta/PI+0.5;

    //c_uv = max(0, 0.4*(3*log(r)+r)+1)*c_uv/length(c_uv);
    //float f = sqrt(sqrt(pow(r/2+1, 4)-1));
    float linear_f = r/2+1.69;
    float log_f = log(r)+2;
    float f = r<1.845?log_f:linear_f;
    c_uv = (f)*c_uv/length(c_uv);
    float r_tranformed = length(c_uv);


    float AA = 10.f/float(BLOCK_SIZE);
    float g = smoothstep(0, AA, max_component(abs(fract(1.6*c_uv)-0.5))-0.43);
    float light = exp(1.0*r+1-0.6*R)/exp(1.f);
    //light *= smoothstep(0.35, 0.5, r);
    light = min(light, 1);
    float swirl = sin01(r_tranformed + a + 0.2*_time + 0.06*sin(10*theta));
    //vec3 swirl_color = mix(vec3(0.34, 0.1, 1), mix(vec3(0.45, 0.5, 1), vec3(0.4, 0.5, 0.8), sin01(a+0.01*_time)), swirl);
    vec3 noisy_color = vec3(mix(vec3(0.5, 0.2, 1), vec3(0.3, 0.7, 1), LayeredSmoothNoise(c_uv)));
    vec3 swirl_color = mix(noisy_color, mix(vec3(0.5, 0.5, 1), vec3(0.3, 0.7, 1), LayeredSmoothNoise(noise_size*vec2(1,1)*sin01(a+0.01*_time))), swirl);
    //vec3 swirl_color = mix(vec3(0.34, 0.1, 0.8), mix(vec3(0.55, 0.5, 0.8), vec3(0.4, 0.6, 0.8), sin01(a+0.01*_time)), swirl);
    
    vec4 forground = blend(vec4(swirl_color, 1), vec4(vec3(0.8), 0.5*g*0.5));
    forground.a = light;
    vec4 color = vec4(0,0,0,1);
    color = blend(color, forground);



    color.a = (1-smoothstep(-1.0, 0, r-R-0.1*sin01(10*a+2*r)));
    gl_FragColor = color;
}