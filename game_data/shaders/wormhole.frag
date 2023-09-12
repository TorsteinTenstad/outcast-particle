#version 120

#include "game_data\\shaders\\include\\standard_uniforms.glsl";
#include "game_data\\shaders\\include\\math_utils.glsl";
#include "game_data\\shaders\\include\\game_constants.glsl";
#include "game_data\\shaders\\include\\blend.glsl";
#include "game_data\\shaders\\include\\rand.glsl";

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
    vec3 noisy_color = vec3(mix(vec3(0.5, 0.2, 1), vec3(0.3, 0.7, 1), layered_smooth_noise(c_uv)));
    vec3 swirl_color = mix(noisy_color, mix(vec3(0.5, 0.5, 1), vec3(0.3, 0.7, 1), layered_smooth_noise(noise_size*vec2(1,1)*sin01(a+0.01*_time))), swirl);
    //vec3 swirl_color = mix(vec3(0.34, 0.1, 0.8), mix(vec3(0.55, 0.5, 0.8), vec3(0.4, 0.6, 0.8), sin01(a+0.01*_time)), swirl);
    
    vec4 forground = blend(vec4(swirl_color, 1), vec4(vec3(0.8), 0.5*g*0.5));
    forground.a = light;
    vec4 color = vec4(0,0,0,1);
    color = blend(color, forground);

    color.a = (1-smoothstep(-1.0, 0, r-R-0.1*sin01(10*a+2*r)));
    gl_FragColor = color;
}