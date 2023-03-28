#version 120

#define PI 3.1415926535897932384626433832795
#define SQRT2 1.41421356237

uniform vec2 _wh;

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


vec4 gem(vec2 uv){
	vec2 gem_c = abs(rot(PI/4)*uv);
	vec2 gem_color_c = (rot(PI/8)*(uv+vec2(0, 0.5)));
	//vec3 gem_color = gem_color_c.x*vec3(0.8, 0.8, 0.1) + gem_color_c.y*vec3(0.9, 0.9, 0.9);
	vec3 gem_color = gem_color_c.x*vec3(0, 1, 1) + gem_color_c.y*vec3(0, 0, 1);
    float AA = 1.f/120.f;
	float gem_alpha = 1-smoothstep(0, AA, max(gem_c.x, gem_c.y)-SQRT2*0.25);
	vec4 gem_contribution = vec4(1-gem_color, gem_alpha);
	return gem_contribution;
}

float coin_size = 120;
float coin_spacing = 60;

uniform int n_collected = 2;

void main()
{
	vec2 uv = gl_TexCoord[0].xy;
    vec2 c_uv = uv-vec2(0, _wh.y/2);

	float margin_ratio = 0.25;
	float gem_size = _wh.y/(1.f+2.f*margin_ratio);
	float margin = gem_size*margin_ratio;
	float gem_r = gem_size/2.f;

    vec4 color = vec4(0.4, 0.4, 0.4, 1);

	color = blend(color, gem((c_uv-vec2(1*margin+1*gem_r,0))/gem_size)*vec4(vec3(1), clamp(float(n_collected)-0.f, 1.20f, 1.f)));
	color = blend(color, gem((c_uv-vec2(2*margin+3*gem_r,0))/gem_size)*vec4(vec3(1), clamp(float(n_collected)-1.f, 1.20f, 1.f)));
	color = blend(color, gem((c_uv-vec2(3*margin+5*gem_r,0))/gem_size)*vec4(vec3(1), clamp(float(n_collected)-2.f, 1.20f, 1.f)));

	gl_FragColor = color * gl_Color;
}