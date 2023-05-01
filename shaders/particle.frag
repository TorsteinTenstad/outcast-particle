#version 120

#define PI 3.14193

uniform float _time;
uniform vec2 _wh;
uniform vec4 inner_color;
uniform vec4 outer_color;
uniform float charge;
uniform float sign_alpha;

#define RED vec3(0.88671875, 0.109375, 0.1015625)
#define LIGHT_RED vec3(0.98046875, 0.6015625, 0.59765625)
#define GREEN vec3(0.19921875, 0.625, 0.171875)
#define LIGHT_GREEN vec3(0.6953125, 0.87109375, 0.5390625)
#define BLUE vec3(0.1171875, 0.46875, 0.703125)
#define LIGHT_BLUE vec3(0.6484375, 0.8046875, 0.88671875)

float particle(vec2 uv, float charge_sign){
	float AA = 0.01;
	float LINES_WIDTH = 0.10;
	float SIGN_SIZE = 0.2;

	float r = length(uv);
	vec2 uv_4 = abs(uv);
	vec2 uv_8 = vec2(max(uv_4.x, uv_4.y), min(uv_4.x, uv_4.y));
	vec2 uv_sign = charge_sign < 0 ? uv_4 : uv_8;
	float pluss_base = smoothstep(0, AA, LINES_WIDTH/2.f-uv_sign.y)
					 * smoothstep(0, AA, SIGN_SIZE-uv_sign.x);
	vec2 rounding_vector = uv_sign-vec2(SIGN_SIZE, 0);
    float pluss_tip = smoothstep(0, AA, LINES_WIDTH/2.f-length(rounding_vector));
	float pluss = max(pluss_base,pluss_tip);
    return pluss;
    float ring_inner_mask = smoothstep(0, AA, r-0.5+LINES_WIDTH);
    float ring_outer_mask = smoothstep(0, AA, 0.5-r);
	float ring = ring_inner_mask*ring_outer_mask;
	return ring+pluss;
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
    vec2 centered_normalized_uv = uv-0.5;
    float r = 2*length(centered_normalized_uv);
    float theta = atan(centered_normalized_uv.x, centered_normalized_uv.y);
    float a = 0.5*theta/PI+0.5;

    float AA = 0.05;
    float outer = smoothstep(0, AA, r-0.3);
    float inner = 1-outer;


    float alpha = smoothstep(0, AA, 1-r);
    float is_red = step(1, charge);
    float is_green = 1-step(-1, charge);
    float is_blue = 1-is_red-is_green;
    float red_alpha = alpha*is_red;
    float green_alpha = alpha*is_green;
    float blue_alpha = alpha*is_blue;

    //vec4 color = mix(vec4(0.98046875, 0.6015625, 0.59765625, alpha), vec4(0.88671875, 0.109375, 0.1015625, alpha), 1-smoothstep(0, 0.2, 0.7-r));
    vec4 color = vec4(0);

    color = blend(color, mix(vec4(RED, red_alpha), vec4(LIGHT_RED, red_alpha), 1-smoothstep(-0.8, 1, r*r)));
    color = blend(color, mix(vec4(GREEN, green_alpha), vec4(LIGHT_GREEN, green_alpha), 1-smoothstep(-0.8, 1, r*r)));
    color = blend(color, mix(vec4(BLUE, blue_alpha), vec4(LIGHT_BLUE, blue_alpha), 1-smoothstep(-0.8, 1, r*r)));
    color = blend(color, vec4(vec3(1), sign_alpha*(1-is_blue)*particle(centered_normalized_uv, sign(charge))));
    gl_FragColor = mix(inner_color, outer_color, r);
    gl_FragColor = vec4(particle(centered_normalized_uv, sign(charge)), a, 0, 1);
    gl_FragColor = gl_Color*color;
}