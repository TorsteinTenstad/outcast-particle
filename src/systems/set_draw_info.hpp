#pragma once

#include "components/draw_info.hpp"
#include "constants.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include "utils.hpp"
#include <cassert>

class SetDrawInfoSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		auto& draw_info_map = level.GetComponent<DrawInfo>();

		for (auto const& [entity_id, charge_dependent_draw_info, charge, draw_info] : level.GetEntitiesWith<ChargeDependentDrawInfo, Charge, DrawInfo>())
		{
			int category = FindClosest(PARTICLE_CHARGE_CATEGORIES, charge->charge);
			draw_info->image_path = PARTICLE_TEXTURES[category];
		}
		for (auto const& [entity_id, player, player_behaviors, charge, draw_info, children, velocity_dependent_draw_layer] : level.GetEntitiesWith<Player, PlayerBehaviors, Charge, DrawInfo, Children, VelocityDependentDrawLayer>())
		{
			float nonzero_charge = 0;
			if (charge->charge != 0)
			{
				int category = FindClosest(PARTICLE_CHARGE_CATEGORIES, charge->charge);
				draw_info->image_path = PLAYER_PARTICLE_TEXTURES[category];
				nonzero_charge = charge->charge;
			}
			else
			{
				int category = FindClosest(PARTICLE_CHARGE_CATEGORIES, player_behaviors->default_charge);
				draw_info->image_path = PLAYER_PARTICLE_NEUTRAL_TEXTURES[category];
				nonzero_charge = player_behaviors->default_charge;
			}

			std::vector<int>& child_ids = children->ids_owned_by_component[typeid(VelocityDependentDrawLayer)];
			if (child_ids.size() > 0)
			{
				assert(nonzero_charge != 0);
				int face = child_ids[0];
				if (nonzero_charge > 0)
				{
					draw_info_map[face].image_path = "content\\textures\\face_plus.png";
				}
				else
				{
					draw_info_map[face].image_path = "content\\textures\\face_minus.png";
				}
			}
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

		for (auto const& [entity_id, magnetic_field, draw_info] : level.GetEntitiesWith<MagneticField, DrawInfo>())
		{
			int category = FindClosest(MAGNETIC_FIELD_STRENGTH_CATEGORIES, magnetic_field->field_strength);
			draw_info->image_path = MAGNETIC_FIELD_TEXTURES[category];
		}

		for (auto const& [entity_id, electric_field, draw_info] : level.GetEntitiesWith<ElectricField, DrawInfo>())
		{
			int category = FindClosest(ELECTRIC_FIELD_STRENGTH_CATEGORIES, Magnitude(electric_field->field_vector));
			draw_info->image_path = ELECTRIC_FIELD_TEXTURES[category];
			if (abs(electric_field->field_vector.x) > abs(electric_field->field_vector.y))
			{
				if (electric_field->field_vector.x > 0)
				{
					draw_info->quarter_turn_rotations = 3;
				}
				else
				{
					draw_info->quarter_turn_rotations = 1;
				}
			}
			else
			{
				if (electric_field->field_vector.y > 0)
				{
					draw_info->quarter_turn_rotations = 0;
				}
				else
				{
					draw_info->quarter_turn_rotations = 2;
				}
			}
		}
	}
	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};