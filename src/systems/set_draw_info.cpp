#include "_pure_DO_systems.hpp"
#include "components/draw_info.hpp"
#include "constants.hpp"
#include "level.hpp"
#include "utils.hpp"
#include <cassert>

void SetDrawInfoSystem::Update(Level& level, float dt)
{
	for (auto const& [entity_id, charge, shader] : level.GetEntitiesWith<Charge, Shader>())
	{
		int category = FindClosest(PARTICLE_CHARGE_CATEGORIES, charge->charge);
		shader->vec4_uniforms["inner_color"] = PARTICLE_INNER_COLOR[category];
		shader->vec4_uniforms["outer_color"] = PARTICLE_OUTER_COLOR[category];
		shader->float_uniforms["charge"] = charge->charge;
		shader->float_uniforms["sign_alpha"] = level.HasComponents<PlayerBehaviors>(entity_id) ? 0 : 1;
	}

	for (auto const& [entity_id, orientation_dependent_drawinfo, width_and_height, draw_info] : level.GetEntitiesWith<OrientationDependentDrawInfo, WidthAndHeight, DrawInfo>())
	{
		if (width_and_height->width_and_height.x > width_and_height->width_and_height.y)
		{
			draw_info->quarter_turn_rotations = 0;
		}
		else
		{
			draw_info->quarter_turn_rotations = 1;
		}
	}

	for (auto const& [entity_id, player, charge] : level.GetEntitiesWith<Player, Charge>())
	{
		for (auto const& [entity_id, electric_field, shader] : level.GetEntitiesWith<ElectricField, Shader>())
		{
			if (!shader->float_uniforms.count("movement_animation_time"))
			{
				shader->float_uniforms["movement_animation_time"] = 0;
			}
			float charge_sign = Sign(charge->charge);
			shader->float_uniforms["charge_sign"] = charge_sign;
			shader->float_uniforms["movement_animation_time"] += charge_sign * dt;
			shader->vec_uniforms["field_vector"] = electric_field->field_vector;
		}
		for (auto const& [entity_id, magnetic_field, shader] : level.GetEntitiesWith<MagneticField, Shader>())
		{
			if (!shader->float_uniforms.count("movement_animation_time"))
			{
				shader->float_uniforms["movement_animation_time"] = 0;
			}
			float charge_sign = Sign(charge->charge);
			shader->float_uniforms["charge_sign"] = charge_sign;
			shader->float_uniforms["movement_animation_time"] += charge_sign * dt;
			shader->float_uniforms["field_strength"] = magnetic_field->field_strength;
		}
	}
}