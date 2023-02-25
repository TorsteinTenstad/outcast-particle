#version 120

uniform float _time;
uniform vec2 _wh;
uniform float charge_sign;
float rand(float seed){ 
    return fract(sin(dot(vec2(seed, fract(seed*55.341)), vec2(12.9898, 78.233))) * 43758.5453);
}

#define COL_W 120
#define PARTICKLE_RING_THICKNESS 0.1

vec4 particles(vec2 uv, float base_speed, float rand_seed){
   	vec2 c = uv/vec2(COL_W);
	vec2 gc = vec2(fract(c.x), c.y);
    float gc_id = floor(c.x);

	float particle_r = 0.3;
	float particle_pluss_size = 0.1;

	float frequency_rand = 0.8+0.2*rand(104.43*sin(gc_id));
    float y = 0.5*base_speed*charge_sign*_time/_wh.y*frequency_rand;
	
	float center_x_rand = rand(gc_id*rand_seed+rand(floor(y)));
	float center_x = particle_r + (1-2*particle_r)*center_x_rand;

    y = fract(y);
    y = smoothstep(0, 1, y);
    y = y*(_wh.y/COL_W+2*particle_r)-particle_r;

    vec2 p = gc - vec2(center_x, y);
	float r = length(p);
	vec2 p_4fold = abs(p);
	vec2 p_8fold = vec2(max(p_4fold.x, p_4fold.y), min(p_4fold.x, p_4fold.y));
	p_8fold = charge_sign<0? p_4fold : p_8fold;
	float AA = 0.01;
	float pluss_base = smoothstep(0, AA, PARTICKLE_RING_THICKNESS/2.f-p_8fold.y)
				* smoothstep(0, AA, particle_pluss_size-p_8fold.x);
	vec2 rounding_vector = p_8fold-vec2(particle_pluss_size, 0);
    float pluss_tip = smoothstep(0, AA, PARTICKLE_RING_THICKNESS/2.f-length(rounding_vector));
	float pluss = max(pluss_base,pluss_tip);
    float ring_inner_mask = smoothstep(0, AA, r-particle_r+PARTICKLE_RING_THICKNESS);
    float ring_outer_mask = smoothstep(0, AA, particle_r-r);
	float ring = ring_inner_mask*ring_outer_mask;
	return vec4(charge_sign>0? vec3(0.5, 0.2, 0.2):vec3(0.2, 0.7, 0.2), 1)*(pluss+ring);
}

void main()
{
   	vec2 c = gl_TexCoord[0].xy/vec2(120);
    vec2 gc = vec2(fract(c.x), c.y);
    float gc_id = floor(c.x);

	float frequency_rand = 0.8+0.2*rand(fract(rand(gc_id)*sin(gc_id*123.123)));
    float time_component = 0*1*_time*frequency_rand;
	float arrow_head_length = 0.1;
	float arrow_c = fract((gc.y-0.5-time_component+0.8*abs(gc.x-0.5)));
	float arrows = 1-smoothstep(0, 0.02, arrow_c-arrow_head_length);
	arrows *= 1-smoothstep(0, 0.02, abs(gc.x-0.5)-0.3);

	vec4 color = vec4(0);
	color += vec4(vec3(1-0.15*float(fract(gc_id/2) > 0.1)), 1)*0.4;
	
	color += vec4(1)*0.2*arrows;
	vec4 particle_contribution = vec4(0);
	particle_contribution = max(particle_contribution, particles(gl_TexCoord[0].xy,200, 2343.12)*0.4);
	particle_contribution = max(particle_contribution, particles(gl_TexCoord[0].xy+vec2(30, 0),600, 8942.71)*0.2);
	particle_contribution = max(particle_contribution, particles(gl_TexCoord[0].xy+vec2(60, 0),800, 6542.11)*0.1);
	particle_contribution = max(particle_contribution, particles(gl_TexCoord[0].xy+vec2(90, 0),400, 1244.51)*0.3);
	color += particle_contribution*vec4(vec3(1), 2);
	gl_FragColor = color;
}