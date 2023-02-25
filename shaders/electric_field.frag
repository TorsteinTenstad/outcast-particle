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

#define CELL_SIZE 90

float particle(vec2 uv, float charge_sign){
	float AA = 0.01;
	float LINES_WIDTH = 0.1;
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

float particles(vec2 uv, float speed, float rand_seed){
   	vec2 c = uv/vec2(CELL_SIZE);
	c.y -= 0.7*speed*movement_animation_time;
	vec2 gc = fract(c)-0.5;
    vec2 id = floor(c);

	float particle_d = 0.6;
	float particle_r = particle_d/2;


	vec2 offset = vec2(0);
	offset.x += rand(-(1-particle_d)/2, (1-particle_d)/2, id*1.234*rand_seed);
	offset.y += rand(-(1-particle_d)/2, (1-particle_d)/2, id*1.312*rand_seed);
	float existance = float(rand01(id*1.623*rand_seed) < 0.1 && fract((id.x+id.y)/2) > 0.4);
	return particle((gc-offset)/particle_d, charge_sign)*existance;
}

void main()
{
	mat2 m_rot = rot(atan(field_vector.x, field_vector.y));

   	vec2 c = gl_TexCoord[0].xy/vec2(120);
	c = c*m_rot;
    vec2 gc = vec2(fract(c.x), c.y);
    float gc_id = floor(c.x);

	float arrow_head_length = 0.1;
	float arrow_c = fract((gc.y-0.5+0.8*abs(gc.x-0.5)));
	float arrows = 1-smoothstep(0, 0.02, arrow_c-arrow_head_length);
	arrows *= 1-smoothstep(0, 0.02, abs(gc.x-0.5)-0.3);

	vec4 color = vec4(0);
	color += vec4(vec3(1-0.15*float(fract(gc_id/2) > 0.1)), 1)*0.4;
	
	color += vec4(1)*0.2*arrows;
	vec4 particle_contribution = vec4(0);
	particle_contribution = max(particle_contribution, particles((gl_TexCoord[0].xy+vec2(90, 0))*m_rot, 2, 1.142)*0.1);
	particle_contribution = max(particle_contribution, particles((gl_TexCoord[0].xy+vec2(60, 0))*m_rot, 4, 1.721)*0.2);
	particle_contribution = max(particle_contribution, particles((gl_TexCoord[0].xy+vec2(30, 0))*m_rot, 6, 1.161)*0.3);
	particle_contribution = max(particle_contribution, particles((gl_TexCoord[0].xy+vec2(00, 0))*m_rot, 8, 1.511)*0.4);
	color += vec4(charge_sign < 0 ? vec3(0.2, 0.8, 0.2) : vec3(0.8, 0.2, 0.2), 1)*particle_contribution;
	gl_FragColor = color;
}