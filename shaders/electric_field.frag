#version 120

#include "shaders\\include\\standard_uniforms.glsl";
#include "shaders\\include\\math_utils.glsl";
#include "shaders\\include\\blend.glsl";
#include "shaders\\include\\game_constants.glsl";
#include "shaders\\include\\colors.glsl";
#include "shaders\\include\\rand.glsl";
#include "shaders\\include\\particles.glsl";

uniform float charge_sign;
uniform float charge_strength;
uniform float movement_animation_time;
uniform vec2 field_vector;

#define CELL_SIZE 90

float particle_grid(vec2 uv, vec2 cell_size, float relative_particle_size, float rand_seed)
{
	vec2 c = uv / vec2(CELL_SIZE);
	vec2 gc = fract(c) - 0.5;
	vec2 id = floor(c);

	float particle_d = relative_particle_size;
	float particle_r = particle_d / 2;

	vec2 offset = vec2(0);
	offset.x += rand(-(1 - particle_d) / 2, (1 - particle_d) / 2, id * 1.234 * rand_seed);
	offset.y += rand(-(1 - particle_d) / 2, (1 - particle_d) / 2, id * 1.312 * rand_seed);
	float existence = float(rand01(id * 1.623 * rand_seed) < 0.08 && fract((id.x + id.y) / 2) > 0.4);
	return particle((gc - offset) / particle_d, charge_sign) * existence;
}

vec4 arrow(vec2 uv, vec2 wh_rotated)
{
	uv = wh_rotated * fract(uv / wh_rotated);
	vec2 c = uv / vec2(120);
	vec2 gc = vec2(fract(c.x), c.y);
	float gc_id = floor(c.x);

	float arrow_head_length = 0.18;
	float arrow_head_width = 0.6;
	float arrow_line_width = 0.13;
	float margin = 0.15;
	float center_dist = abs(gc.x - 0.5);
	float arrow_c = gc.y - (wh_rotated.y / 120) + 1 * center_dist;

	float AA = 0.02;

	float arrow_back_mask = 1 - smoothstep(0, AA, arrow_c + margin);
	float arrow_front_mask = smoothstep(0, AA, arrow_c + arrow_head_length + margin);
	float arrow_width_mask = 1 - smoothstep(0, AA, center_dist - arrow_head_width / 2);

	float lines_mask = 1 - smoothstep(0, AA, center_dist - arrow_line_width / 2);
	lines_mask *= arrow_back_mask;
	lines_mask *= smoothstep(0, AA, c.y - margin);

	float arrow = max(arrow_front_mask * arrow_back_mask * arrow_width_mask, lines_mask);
	//float gradient_t = 1-smoothstep(0, 100, wh_rotated.y -uv.y);
	float gradient_t = smoothstep(0, 1, 1 - smoothstep(wh_rotated.y, 0, uv.y));
	//float gradient_t = 1/(1+(wh_rotated.y - uv.y - margin*120)/10);
	gradient_t = clamp(gradient_t, 0, 1);
	vec3 rgb = mix(vec3(0.17), vec3(0.12), gradient_t);
	return vec4(rgb, arrow);
}

void main()
{
	vec2 xy = gl_TexCoord[0].xy * _wh;
	float theta = atan(field_vector.x, field_vector.y);
	mat2 m_rot = rot(theta);

	vec4 background_color = vec4(vec3(1), 0.0);
	vec4 color = background_color;

	vec3 particle_rgb = get_flat_particle_color(charge_sign);
	vec4 particles_color = vec4(0);
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particle_grid((xy) * m_rot + vec2(90, -100 * movement_animation_time), vec2(CELL_SIZE), 0.6, 1.142) * 0.1 * 0.5));
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particle_grid((xy) * m_rot + vec2(60, -200 * movement_animation_time), vec2(CELL_SIZE), 0.6, 1.721) * 0.2 * 0.5));
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particle_grid((xy) * m_rot + vec2(30, -200 * movement_animation_time), vec2(CELL_SIZE), 0.6, 1.161) * 0.3 * 0.5));
	particles_color = blend(particles_color, vec4(vec3(particle_rgb), particle_grid((xy) * m_rot + vec2(00, -400 * movement_animation_time), vec2(CELL_SIZE), 0.6, 1.511) * 0.4 * 0.5));

	vec4 arrow_color = arrow((xy) * m_rot, abs(_wh * m_rot));
	color = blend(color, arrow_color);

	color = blend(color, particles_color);

	gl_FragColor = color;
}