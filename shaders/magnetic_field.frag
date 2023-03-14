#version 120

#define PI 3.1415926535897932384626433832795

#define GRID_SIZE 120

uniform float _time;
uniform vec2 _wh;
uniform float charge_sign;
uniform float movement_animation_time;
uniform float field_strength;


uniform sampler2D _noise_texture;
#define noise_size 1000
float rand01(vec2 uv){
    uv = fract(uv/noise_size);
    return texture2D(_noise_texture, 1-uv).r;
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

float particle(vec2 uv, mat2 m_rot, float charge_sign){
	float AA = 0.01;
	float LINES_WIDTH = 0.15;
	float SIGN_SIZE = 0.2;

	float r = length(uv);
	vec2 uv_4 = abs(m_rot*uv);
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

float particle_grid(vec2 uv, mat2 m_rot, vec2 cell_size, float n, float relative_particle_size, vec2 rand_seed){
   	vec2 c = uv/cell_size;
	vec2 gc = fract(c)-0.5;
    vec2 id = floor(c);
    if (id.x < 0 || id.y < 0 || id.x > (n-0.5) || id.y > (n-0.5)){
        return 0;
    }

	float particle_d = relative_particle_size;
	float particle_r = particle_d/2;


	vec2 offset = vec2(0);
	offset.x += rand(-(1-particle_d)/2, (1-particle_d)/2, id+1.234+rand_seed);
	offset.y += rand(-(1-particle_d)/2, (1-particle_d)/2, id+1.312+rand_seed);
	float existance = float(rand01(id+1.623+rand_seed) < 0.1 && fract((id.x+id.y)/2) < 0.3);
	return particle(((gc-offset)/particle_d), m_rot, charge_sign)*existance;
}

float particles(vec2 uv, float particles_per_cell_side, float speed, float rand_seed){
    float particle_cell_size = CELL_SIZE;
    float rotate_group_size = particles_per_cell_side*particle_cell_size;
    vec2 c = uv/rotate_group_size;
	vec2 gc = fract(c);
    vec2 id = floor(c);
    float n = floor(particles_per_cell_side/sqrt(2));
    float theta = sign(field_strength)*movement_animation_time*speed/(particles_per_cell_side*particles_per_cell_side);
    gc = rot(theta)*(gc-0.5)+0.5;
    gc = gc-0.5+(n/(2*particles_per_cell_side));
    return particle_grid(gc, rot(-theta), vec2(1/particles_per_cell_side), n, 0.6, rand_seed+id);
}

vec4 crosses_and_dots(vec2 uv){
   	vec2 c = uv/vec2(GRID_SIZE);
    vec2 gc = fract(c);
    float gc_id = floor(c.x);

    gc = rot(PI/4)*(gc-0.5);
    
	vec2 gc_4 = abs(gc);
	vec2 gc_8 = vec2(max(gc_4.x, gc_4.y), min(gc_4.x, gc_4.y));
    
    float AA = 0.02;
    float line_width = 0.15;
	float crosses_size = 0.35;
	float crosses_mask = smoothstep(0, AA, line_width/2.f-gc_8.y)
		               * smoothstep(0, AA, crosses_size-gc_8.x);

    float dots_mask = smoothstep(0, AA, 0.15-length(gc));

	float alpha = field_strength > 0 ? dots_mask : crosses_mask;
	vec3 light_purple = vec3(0.7890625, 0.6953125, 0.8359375);
    vec3 purple = vec3(0.59375, 0.3046875, 0.63671875);
	vec3 rgb = mix(vec3(0.17), vec3(0.12), 1-(gc.x+gc.y));
	return vec4(rgb, alpha);
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

	vec4 background_color = vec4(vec3(1), 0);
	vec4 color = background_color;

	
    vec3 red = vec3(0.88671875, 0.109375, 0.1015625);
    vec3 light_red = vec3(0.98046875, 0.6015625, 0.59765625);
    vec3 green = vec3(0.19921875, 0.625, 0.171875);
    vec3 light_green = vec3(0.6953125, 0.87109375, 0.5390625);
    vec3 blue = vec3(0.1171875, 0.46875, 0.703125);
    vec3 light_blue = vec3(0.6484375, 0.8046875, 0.88671875);

    vec3 particle_rgb = charge_sign < 0 ? (0.2*light_green+0.8*green) : (0.2*light_red+0.8*red);
	vec4 particles_color = vec4(0);
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particles((uv+vec2(90, 0)), 12, 20, 1.142)*0.1*0.5));
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particles((uv+vec2(90, 0)), 10, 20, 1.142)*0.1*0.5));
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particles((uv+vec2(60, 0)), 10, 40, 1.721)*0.2*0.5));
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particles((uv+vec2(60, 0)),  8, 40, 1.721)*0.2*0.5));
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particles((uv+vec2(30, 0)),  8, 60, 1.161)*0.3*0.5));
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particles((uv+vec2(30, 0)),  6, 60, 1.161)*0.3*0.5));
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particles((uv+vec2(00, 0)),  6, 80, 1.511)*0.4*0.5));
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particles((uv+vec2(00, 0)),  4, 80, 1.511)*0.4*0.5));
	
	color = blend(color, crosses_and_dots((uv)));
	color = blend(color, particles_color);

	gl_FragColor = color;
}