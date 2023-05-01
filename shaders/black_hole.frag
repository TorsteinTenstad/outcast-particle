#version 120

#include "shaders\\include\\standard_uniforms.glsl";
#include "shaders\\include\\math_utils.glsl";

#define scale 1
#define sweep 4
#define rotation_speed 0.12
#define n 8
#define falloff 1000
#define center_r 0.25
#define unrotated_falloff 0.6

void main()
{
    vec2 p = (gl_TexCoord[0].xy/_wh-0.5)/scale;
    vec2 v = rot(-rotation_speed*_time+sweep*length(p))*p;
    float theta = atan(v.x, v.y);
    float a = 0.5*theta/PI+0.5;
    float a_rep = fract(n*a);
    //float peaks = pow(abs(2.0*(a_rep-0.5)),1.2);
    float peaks = sin(2*PI*a*n);
    float val = 1/(1+max(0,falloff*(length(v)-center_r-0.05*peaks)));
    val = (smoothstep(0.2, 0.8, val));

    vec3 col = mix(vec3(0.5,0.5,1), vec3(0.0,0,1), 0.00001*length(v)-0.6);
    col += 1.0-smoothstep(0.0, 0.8, 0.01*length(v));
    col *= smoothstep(0.3, 0.6, 0.01*length(v));
    
    vec3 final_col = mix(vec3(0), col, val);
    

    // Output to screen
    gl_FragColor = vec4(vec3(0),val);
    //gl_FragColor = vec4((final_col),1);
}