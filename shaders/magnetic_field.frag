#version 120

#include "shaders\\include\\standard_uniforms.glsl";
#include "shaders\\include\\math_utils.glsl";
#include "shaders\\include\\game_constants.glsl";
#include "shaders\\include\\colors.glsl";
#include "shaders\\include\\rand.glsl";
#include "shaders\\include\\particles.glsl";
#include "shaders\\include\\blend.glsl";
#include "shaders\\include\\fields.glsl";


uniform float charge_sign;
uniform float movement_animation_time;
uniform float field_strength;

#define CELL_SIZE 90
#define MAX_FIELD_STRENGTH 0.8
#define MIN_FIELD_STRENGTH 0.05

float particle_grid(vec2 xy, mat2 m_rot, vec2 cell_size, float n, float relative_particle_size, vec2 rand_seed)
{
	vec2 c = xy / cell_size;
	vec2 gc = fract(c) - 0.5;
	vec2 id = floor(c);
	if (id.x < 0 || id.y < 0 || id.x > (n - 0.5) || id.y > (n - 0.5))
	{
		return 0;
	}

	float particle_d = relative_particle_size;
	float particle_r = particle_d / 2;

	vec2 offset = vec2(0);
	offset.x += rand(-(1 - particle_d) / 2, (1 - particle_d) / 2, id + 1.234 + rand_seed);
	offset.y += rand(-(1 - particle_d) / 2, (1 - particle_d) / 2, id + 1.312 + rand_seed);
	float existence = float(rand01(id + 1.623 + rand_seed) < 0.1 && fract((id.x + id.y) / 2) < 0.3);
	return particle(m_rot*((gc - offset) / particle_d), charge_sign) * existence;
}

float particles(vec2 xy, float particles_per_cell_side, float speed, float rand_seed)
{
	float particle_cell_size = CELL_SIZE;
	float rotate_group_size = particles_per_cell_side * particle_cell_size;
	vec2 c = xy / rotate_group_size;
	vec2 gc = fract(c);
	vec2 id = floor(c);
	float n = floor(particles_per_cell_side / sqrt(2));
	float theta = sign(field_strength) * movement_animation_time * speed / (particles_per_cell_side * particles_per_cell_side);
	gc = rot(theta) * (gc - 0.5) + 0.5;
	gc = gc - 0.5 + (n / (2 * particles_per_cell_side));
	return particle_grid(gc, rot(-theta), vec2(1 / particles_per_cell_side), n, 0.6, rand_seed + id);
}

float crosses_and_dots(vec2 xy, float thickness_factor)
{
	vec2 c = xy / vec2(BLOCK_SIZE);
	vec2 gc = fract(c);
	float gc_id = floor(c.x);

	gc = rot(PI / 4) * (gc - 0.5);

	vec2 gc_4 = abs(gc);
	vec2 gc_8 = vec2(max(gc_4.x, gc_4.y), min(gc_4.x, gc_4.y));

	float AA = 0.02;
	float line_width = 0.15*thickness_factor;
	float crosses_size = 0.35;
	float crosses_mask = smoothstep(0, AA, line_width / 2.f - gc_8.y) * smoothstep(0, AA, crosses_size - gc_8.x);

	float dots_mask = smoothstep(0, AA, line_width - length(gc));

	return field_strength > 0 ? dots_mask : crosses_mask;
}

void main()
{
	vec2 uv = gl_TexCoord[0].xy;
	vec2 xy = uv * _wh;

	vec4 background_color = vec4(vec3(1), 0);
	vec4 color = background_color;

	vec3 particle_rgb = get_flat_particle_color(charge_sign);
	vec4 particles_color = vec4(0);
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particles((xy + vec2(90, 0)), 12, 20, 1.142) * 0.1 * 1.5));
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particles((xy + vec2(90, 0)), 10, 20, 1.142) * 0.1 * 1.5));
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particles((xy + vec2(60, 0)), 10, 40, 1.721) * 0.2 * 1.5));
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particles((xy + vec2(60, 0)), 8, 40, 1.721) * 0.2 * 1.5));
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particles((xy + vec2(30, 0)), 8, 60, 1.161) * 0.3 * 1.5));
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particles((xy + vec2(30, 0)), 6, 60, 1.161) * 0.3 * 1.5));
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particles((xy + vec2(00, 0)), 6, 80, 1.511) * 0.4 * 1.5));
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particles((xy + vec2(00, 0)), 4, 80, 1.511) * 0.4 * 1.5));

	float field_strength01 = get_linearized_log2_field_strength01(field_strength, MIN_FIELD_STRENGTH, MAX_FIELD_STRENGTH);
	vec3 field_rgb = get_field_rgb(uv, field_strength01);
	float thickness_factor = get_field_thickness_factor(field_strength01);
	float crosses_and_dots_mask = crosses_and_dots(xy, thickness_factor);

	color = blend(color, vec4(field_rgb, crosses_and_dots_mask));
	color = blend(color, particles_color);

	gl_FragColor = color;
}