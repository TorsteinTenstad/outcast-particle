#version 120

#define PI 3.1415926535897932384626433832795
#define SQRT2 1.41421356237

uniform vec2 _wh;
uniform float _time;

vec4 blend(vec4 base, vec4 top){
	float a = top.a + base.a * (1-top.a);
	if (abs(a) < 0.0001){
		return vec4(0);
	}
	vec3 rgb = (top.a*top.rgb+base.a*base.rgb*(1-top.a))/a;
	return vec4(rgb, a);
}

mat2 rot(float theta) {
	float s = sin(theta);
	float c = cos(theta);
	mat2 m = mat2(c, -s, s, c);
    return m;
}

float radial_falloff(float r, float a, float b){
	float f = 1/(max((r-a), 0)/(b-a)+1);
	return smoothstep(0.5, 1, f);
}

float twinkle(vec2 uv){
	return 0.01/abs(uv.x*uv.y);
}

float twinkle(vec2 uv, float theta, float intensity){
	uv = rot(theta)*2*(uv - 0.5);
	return intensity*twinkle(uv)*radial_falloff(length(uv), 0, 0.5+0.5*smoothstep(0, 1, intensity));
}

void main()
{
	vec2 uv = gl_TexCoord[0].xy;

	gl_FragColor = vec4(vec3(1), twinkle(uv/_wh.y, 1.1*_time*PI/4.0, 0.5+0.5*sin(4*_time)));
}