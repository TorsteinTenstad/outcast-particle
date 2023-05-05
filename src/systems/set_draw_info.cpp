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

static int CreateStrengthIndicator(Level& level)
{
	EntityHandle handle = CreateTexturedRectangle(level, sf::Vector2f(0, 0), sf::Vector2f(1, 1) * 0.75f * float(BLOCK_SIZE), 0, "", false);
	int id = GetId(handle);
	level.AddComponent<Shader>(id)->fragment_shader_path = "shaders\\strength_indicator.frag";
	return id;
}

class StrengthIndicator
{};

static void UpdateStrengthIndicator(Level& level, int entity, int category_idx)
{
	int indicator_id = GetSingletonChildId<StrengthIndicator>(level, entity, CreateStrengthIndicator);
	level.GetComponent<Shader>(indicator_id)->float_uniforms["category_idx"] = category_idx;
	sf::Vector2f top_right_offset = GetSize(level, entity, false) / 2.f;
	top_right_offset -= level.GetComponent<WidthAndHeight>(indicator_id)->width_and_height / 2.f;
	top_right_offset.y *= -1;
	level.GetComponent<Position>(indicator_id)->position = level.GetComponent<Position>(entity)->position + top_right_offset;
	level.GetComponent<DrawPriority>(indicator_id)->draw_priority = level.GetComponent<DrawPriority>(entity)->draw_priority;
}

class StaticIndicator
{};

static int CreateStaticIndicator(Level& level)
{
	EntityHandle handle = CreateTexturedRectangle(level, sf::Vector2f(0, 0), sf::Vector2f(1, 1) * 0.5f * float(BLOCK_SIZE), 0, "content\\textures\\lock.png", false);
	int id = GetId(handle);
	level.AddComponent<StaticIndicator>(id);
	return id;
}

static void UpdateStaticIndicator(Level& level, int entity)
{
	int indicator_id = GetSingletonChildId<StaticIndicator>(level, entity, CreateStaticIndicator);
	sf::Vector2f top_right_offset = GetSize(level, entity, false) / 2.f;
	top_right_offset -= level.GetComponent<WidthAndHeight>(indicator_id)->width_and_height / 2.f;
	top_right_offset.x *= -1;
	top_right_offset.y *= -1;
	level.GetComponent<Position>(indicator_id)->position = level.GetComponent<Position>(entity)->position + top_right_offset;
	level.GetComponent<DrawPriority>(indicator_id)->draw_priority = level.GetComponent<DrawPriority>(entity)->draw_priority;
}

void SetDrawInfoSystem::Update(Level& level, float dt)
{
	if (level.GetMode() == EDIT_MODE)
	{
		for (auto [entity, component] : level.GetEntitiesWith<Charge>())
		{
			if (level.HasComponents<ReceivedForces>(entity)) { continue; }
			UpdateStaticIndicator(level, entity);
		}
	}
	else
	{
		level.DeleteEntitiesWith<StaticIndicator>();
	}
	for (auto [entity, component] : level.GetEntitiesWith<Charge>())
	{
		if (PlayerBehaviors* player_behaviors = level.RawGetComponent<PlayerBehaviors>(entity))
		{
			if (player_behaviors->is_neutral)
			{
				DeleteChildrenOwnedBy<StrengthIndicator>(level, entity);
				continue;
			}
		}
		unsigned category_idx = FindClosest(PARTICLE_CHARGE_CATEGORIES, abs(component->charge));
		UpdateStrengthIndicator(level, entity, category_idx);
	}
	for (auto [entity, component] : level.GetEntitiesWith<ElectricField>())
	{
		unsigned category_idx = FindClosest(ELECTRIC_FIELD_STRENGTH_CATEGORIES, Magnitude(component->field_vector));
		UpdateStrengthIndicator(level, entity, category_idx);
	}
	for (auto [entity, component] : level.GetEntitiesWith<MagneticField>())
	{
		unsigned category_idx = FindClosest(MAGNETIC_FIELD_STRENGTH_CATEGORIES, abs(component->field_strength));
		UpdateStrengthIndicator(level, entity, category_idx);
	}

	for (auto const& [entity_id, charge, shader] : level.GetEntitiesWith<Charge, Shader>())
	{
		shader->float_uniforms["charge"] = charge->charge;
		shader->float_uniforms["sign_alpha"] = level.HasComponents<PlayerBehaviors>(entity_id) ? 0 : 1;
	}

	for (auto const& [entity_id, orientation_dependent_draw_info, width_and_height, draw_info] : level.GetEntitiesWith<OrientationDependentDrawInfo, WidthAndHeight, DrawInfo>())
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