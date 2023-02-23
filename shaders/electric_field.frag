#version 120

uniform float _time;
uniform vec2 _wh;
uniform float charge_sign;
float rand(float seed){ 
    return fract(sin(dot(vec2(seed, fract(seed*0.341)), vec2(12.9898, 78.233))) * 43758.5453);
}

#define D 40
#define PARTICKLE_RING_THICKNESS 0.1
#define PARTICKLE_PLUSS_SIZE 0.2

vec4 particles(vec2 uv){
   	vec2 c = uv/vec2(D);
	vec2 gc = vec2(fract(c.x), c.y);
    float gc_id = floor(c.x);
	float frequency_rand = 0.8+0.2*rand(gc_id);
    float y = 500*charge_sign*_time/_wh.y*frequency_rand;
    y = fract(y);
    y = smoothstep(0, 1, y);
    y = y*(_wh.y/D+1.f)-0.5;
    vec2 p = gc - vec2(0.5, y);
	float r = length(p);
	vec2 p_4fold = abs(p);
	vec2 p_8fold = vec2(max(p_4fold.x, p_4fold.y), min(p_4fold.x, p_4fold.y));
	p_8fold = charge_sign<0? p_4fold : p_8fold;
	float pluss_base = smoothstep(0, 0.002, PARTICKLE_RING_THICKNESS/2.f-p_8fold.y)
				* smoothstep(0, 0.002, PARTICKLE_PLUSS_SIZE-p_8fold.x);
	vec2 rounding_vector = p_8fold-vec2(PARTICKLE_PLUSS_SIZE, 0);
    float pluss_tip = smoothstep(0, 0.002, PARTICKLE_RING_THICKNESS/2.f-length(rounding_vector));
	float pluss = pluss_base+pluss_tip;
    float ring_inner_mask = smoothstep(0, 0.002, r-0.5+PARTICKLE_RING_THICKNESS);
    float ring_outer_mask = smoothstep(0, 0.002, 0.5-r);
	float ring = ring_inner_mask*ring_outer_mask;
	return vec4(charge_sign>0? vec3(0.7, 0.2, 0.2):vec3(0.2, 0.7, 0.2), 1)*(pluss+ring);
}

void main()
{
   	vec2 c = gl_TexCoord[0].xy/vec2(120);
    vec2 gc = vec2(fract(c.x), c.y);
    float gc_id = floor(c.x);

	float frequency_rand = 0.8+0.2*rand(fract(rand(gc_id)*sin(gc_id*123.123)));
    float time_component = 1*_time*frequency_rand;
	float arrow_c = 3*fract((gc.y-time_component+0.8*abs(gc.x-0.5))/3);
	float arrows = 1-smoothstep(0, 0.002, arrow_c-0.5);

	vec4 color = vec4(vec3(1-0.4*float(fract(gc_id/2) > 0.1)), 1)*0.3;
	
	color += vec4(1)*0.4*arrows;
	color += particles(gl_TexCoord[0].xy);
	gl_FragColor = color;
}