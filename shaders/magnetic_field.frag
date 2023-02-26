#version 120

#define PI 3.1415926535897932384626433832795

#define GRID_SIZE 120

uniform float _time;
uniform vec2 _wh;
uniform float charge_sign;
uniform float movement_animation_time;
uniform float field_strength;

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

#define CELL_SIZE 60

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

float particles(vec2 uv, float speed, float rand_seed){
    float n = 5;
    vec2 rot_c = uv/(n*CELL_SIZE);
    vec2 rot_gc = fract(rot_c);

    float total = 0;
    for (int i=-1; i<=1; i++){
        for (int j=-1; j<=1; j++){
            if (i==0&&j==0){
                continue;
            }

            vec2 rot_id = floor(rot_c);

            rot_gc = rot(_time)*(rot_gc-0.5)+0.5;

            vec2 c = n*rot_gc;
            vec2 gc = fract(c)-0.5;
            vec2 inner_id = floor(c);
            vec2 id = inner_id + n*rot_id;

            float particle_d = 0.6;
            float particle_r = particle_d/2;
            vec2 offset = vec2(0);
            offset.x += rand(-(1-particle_d)/2, (1-particle_d)/2, id*1.234*rand_seed);
            offset.y += rand(-(1-particle_d)/2, (1-particle_d)/2, id*1.312*rand_seed);
            float existance = float(rand01(id*1.623*rand_seed) < 0.1 && fract((id.x+id.y)/2) > 0.4);
            existance *= float(length(floor(inner_id*5)) > 0.00001);
            total = max(total, particle((gc-offset)/particle_d, charge_sign)*existance);
        }
    }
    return total;
}

float crosses_and_dots(vec2 uv){
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

    return field_strength > 0 ? dots_mask : crosses_mask;
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

	vec4 background_color = vec4(vec3(1), 0.1);
	vec4 color = background_color;

    vec3 particle_rgb = charge_sign < 0 ? vec3(0.3, 0.8, 0.3) : vec3(0.95, 0.3, 0.3);
	vec4 particles_color = vec4(0);
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particles((uv+vec2(90, 0)), 2, 1.142)*0.1));
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particles((uv+vec2(60, 0)), 4, 1.721)*0.2));
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particles((uv+vec2(30, 0)), 6, 1.161)*0.3));
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particles((uv+vec2(00, 0)), 8, 1.511)*0.4));
	
	color = blend(color, particles_color);

	vec4 arrow_color = vec4(vec3(0.8), 1)*crosses_and_dots((uv));
	color = blend(color, arrow_color);

	gl_FragColor = color;
}