#version 120

uniform float _time;
uniform vec2 _wh;
uniform float charge_sign;
uniform float movement_animation_time;
uniform vec2 field_vector;

float rand01(vec2 seed){
    return fract(sin(dot(seed, vec2(12.9898, 78.233))) * 43758.5453);
}

float rand(float a, float b, vec2 seed){
	return a + rand01(seed)*(b-a);
}

mat2 rot(float a) {
	float s = sin(a);
	float c = cos(a);
	mat2 m = mat2(c, -s, s, c);
    return m;
}

float particle(vec2 uv, float charge_sign){
	float AA = 0.01;
	float LINES_WIDTH = 0.15;
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
    float ring_inner_mask = smoothstep(0, AA, r-0.5+LINES_WIDTH);
    float ring_outer_mask = smoothstep(0, AA, 0.5-r);
	float ring = ring_inner_mask*ring_outer_mask;
	return ring+pluss;
}

#define CELL_SIZE 90

float particle_grid(vec2 uv, vec2 cell_size, float relative_particle_size, float rand_seed){
   	vec2 c = uv/vec2(CELL_SIZE);
	vec2 gc = fract(c)-0.5;
    vec2 id = floor(c);

	float particle_d = relative_particle_size;
	float particle_r = particle_d/2;


	vec2 offset = vec2(0);
	offset.x += rand(-(1-particle_d)/2, (1-particle_d)/2, id*1.234*rand_seed);
	offset.y += rand(-(1-particle_d)/2, (1-particle_d)/2, id*1.312*rand_seed);
	float existance = float(rand01(id*1.623*rand_seed) < 0.08 && fract((id.x+id.y)/2) > 0.4);
	return particle((gc-offset)/particle_d, charge_sign)*existance;
}

vec4 arrow(vec2 uv, vec2 wh_rotated){
	uv = wh_rotated*fract(uv/wh_rotated);
   	vec2 c = uv/vec2(120);
    vec2 gc = vec2(fract(c.x), c.y);
    float gc_id = floor(c.x);

	float arrow_head_length = 0.18;
	float arrow_head_width = 0.6;
    float arrow_line_width = 0.13;
	float margin = 0.15;
	float center_dist = abs(gc.x-0.5);
	float arrow_c = gc.y-(wh_rotated.y/120)+1*center_dist;

	float AA = 0.02;

	float arrow_back_mask = 1-smoothstep(0, AA, arrow_c+margin);
	float arrow_front_mask = smoothstep(0, AA, arrow_c+arrow_head_length+margin);
	float arrow_width_mask = 1-smoothstep(0, AA, center_dist-arrow_head_width/2);

	float lines_mask = 1-smoothstep(0, AA, center_dist-arrow_line_width/2);
	lines_mask *= arrow_back_mask;
	lines_mask *= smoothstep(0, AA, c.y - margin);

	float arrow = max(arrow_front_mask*arrow_back_mask*arrow_width_mask, lines_mask);
	//float gradient_t = 1-smoothstep(0, 100, wh_rotated.y -uv.y);
	float gradient_t = smoothstep(0, 1, 1-smoothstep(wh_rotated.y, 0, uv.y));
	//float gradient_t = 1/(1+(wh_rotated.y - uv.y - margin*120)/10);
	gradient_t = clamp(gradient_t, 0, 1);
	vec3 rgb = mix(vec3(0.17), vec3(0.12), gradient_t);
	return vec4(rgb, arrow);
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
	float theta = atan(field_vector.x, field_vector.y);
	mat2 m_rot = rot(theta);

	vec4 background_color = vec4(vec3(1), 0.0);
	vec4 color = background_color;

	vec3 red = vec3(0.88671875, 0.109375, 0.1015625);
    vec3 light_red = vec3(0.98046875, 0.6015625, 0.59765625);
    vec3 green = vec3(0.19921875, 0.625, 0.171875);
    vec3 light_green = vec3(0.6953125, 0.87109375, 0.5390625);
    vec3 blue = vec3(0.1171875, 0.46875, 0.703125);
    vec3 light_blue = vec3(0.6484375, 0.8046875, 0.88671875);

    vec3 particle_rgb = charge_sign < 0 ? (0.2*light_green+0.8*green) : (0.2*light_red+0.8*red);
	vec4 particles_color = vec4(0);
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particle_grid((gl_TexCoord[0].xy)*m_rot+vec2(90, -100*movement_animation_time), vec2(CELL_SIZE), 0.6, 1.142)*0.1*0.5));
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particle_grid((gl_TexCoord[0].xy)*m_rot+vec2(60, -200*movement_animation_time), vec2(CELL_SIZE), 0.6, 1.721)*0.2*0.5));
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particle_grid((gl_TexCoord[0].xy)*m_rot+vec2(30, -200*movement_animation_time), vec2(CELL_SIZE), 0.6, 1.161)*0.3*0.5));
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particle_grid((gl_TexCoord[0].xy)*m_rot+vec2(00, -400*movement_animation_time), vec2(CELL_SIZE), 0.6, 1.511)*0.4*0.5));
	

	vec4 arrow_color = arrow((gl_TexCoord[0].xy)*m_rot, abs(_wh*m_rot));
	color = blend(color, arrow_color);

	color = blend(color, particles_color);
	
	gl_FragColor = color;
}