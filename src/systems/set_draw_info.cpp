#include "components/draw_info.hpp"
#include "components/physics.hpp"
#include "components/player.hpp"
#include "components/size.hpp"
#include "constants.hpp"
#include "entity_creation.hpp"
#include "level.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "utils/container_operations.hpp"
#include "utils/get_size.hpp"
#include "utils/math.hpp"

class StrengthIndicator
{};

static Entity CreateStrengthIndicator(Level& level)
{
	EntityHandle handle = CreateTexturedRectangle(level, sf::Vector2f(0, 0), sf::Vector2f(1, 1) * 0.75f * float(BLOCK_SIZE), 0, "", false);
	Entity entity = GetEntity(handle);
	level.AddComponent<Shader>(entity)->fragment_shader_path = "shaders\\strength_indicator.frag";
	level.AddComponent<StrengthIndicator>(entity);
	return entity;
}

static void UpdateStrengthIndicator(Level& level, Entity entity, unsigned category_idx)
{
	Entity indicator_entity = GetSingletonChildId<Charge>(level, entity, CreateStrengthIndicator);
	level.GetComponent<Shader>(indicator_entity)->float_uniforms["category_idx"] = category_idx;
	sf::Vector2f top_right_offset = GetSize(level, entity, false) / 2.f;
	top_right_offset -= level.GetComponent<WidthAndHeight>(indicator_entity)->width_and_height / 2.f;
	top_right_offset.y *= -1;
	level.GetComponent<Position>(indicator_entity)->position = level.GetComponent<Position>(entity)->position + top_right_offset;
	level.GetComponent<DrawPriority>(indicator_entity)->draw_priority = level.GetComponent<DrawPriority>(entity)->draw_priority;
}

class StaticIndicator
{};

static Entity CreateStaticIndicator(Level& level)
{
	EntityHandle handle = CreateTexturedRectangle(level, sf::Vector2f(0, 0), sf::Vector2f(1, 1) * 0.5f * float(BLOCK_SIZE), 0, "content\\textures\\lock.png", false);
	Entity entity = GetEntity(handle);
	level.AddComponent<StaticIndicator>(entity);
	return entity;
}

static void UpdateStaticIndicator(Level& level, Entity entity)
{
	Entity indicator_entity = GetSingletonChildId<StaticIndicator>(level, entity, CreateStaticIndicator);
	sf::Vector2f top_right_offset = GetSize(level, entity, false) / 2.f;
	top_right_offset -= level.GetComponent<WidthAndHeight>(indicator_entity)->width_and_height / 2.f;
	top_right_offset.x *= -1;
	top_right_offset.y *= -1;
	level.GetComponent<Position>(indicator_entity)->position = level.GetComponent<Position>(entity)->position + top_right_offset;
	level.GetComponent<DrawPriority>(indicator_entity)->draw_priority = level.GetComponent<DrawPriority>(entity)->draw_priority;
}

void SetDrawInfoSystem::Update(Level& level, float dt)
{
	if (level.GetMode() == EDIT_MODE)
	{
		for (auto [entity, component] : level.GetEntitiesWith<Charge>())
		{
			unsigned category_idx = FindClosest(PARTICLE_CHARGE_CATEGORIES, abs(component->charge));
			UpdateStrengthIndicator(level, entity, category_idx);

			if (level.HasComponents<ReceivedForces>(entity)) { continue; }
			UpdateStaticIndicator(level, entity);

			if (PlayerBehaviors* player_behaviors = level.RawGetComponent<PlayerBehaviors>(entity))
			{
				if (player_behaviors->is_neutral)
				{
					DeleteChildrenOwnedBy<Charge>(level, entity);
					continue;
				}
			}
		}
	}
	else
	{
		level.DeleteEntitiesWith<StaticIndicator>();
		level.DeleteEntitiesWith<StrengthIndicator>();
	}

	for (auto const& [entity, charge, shader] : level.GetEntitiesWith<Charge, Shader>())
	{
		shader->float_uniforms["charge"] = charge->charge;
		shader->float_uniforms["sign_alpha"] = level.HasComponents<PlayerBehaviors>(entity) ? 0 : 1;
	}

	for (auto const& [entity, orientation_dependent_draw_info, width_and_height, draw_info] : level.GetEntitiesWith<OrientationDependentDrawInfo, WidthAndHeight, DrawInfo>())
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

	for (auto const& [entity, player, charge] : level.GetEntitiesWith<Player, Charge>())
	{
		for (auto const& [entity, electric_field, shader] : level.GetEntitiesWith<ElectricField, Shader>())
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
		for (auto const& [entity, magnetic_field, shader] : level.GetEntitiesWith<MagneticField, Shader>())
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