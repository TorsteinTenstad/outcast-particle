#pragma once
#include "PCH.hpp"
#include "components/draw_info.hpp"
#include "constants.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include "utils.hpp"

class SetDrawInfoSystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		auto& draw_info_map = level.GetComponent<DrawInfo>();
		auto& charge_dependent_drawinfo_map = level.GetComponent<ChargeDependentDrawInfo>();
		auto& orientation_dependent_drawinfo_map = level.GetComponent<OrientationDependentDrawInfo>();
		auto& charge_map = level.GetComponent<Charge>();
		auto& width_and_height_map = level.GetComponent<WidthAndHeight>();
		auto& electric_field_map = level.GetComponent<ElectricField>();
		auto& magnetic_field_map = level.GetComponent<MagneticField>();
		auto& player_map = level.GetComponent<Player>();

		for (auto const& [entity_id, charge_dependent_drawinfo] : charge_dependent_drawinfo_map)
		{
			int category = FindClosest(PARTICLE_CHARGE_CATEGORIES, charge_map[entity_id].charge);
			if (player_map.count(entity_id))
			{
				draw_info_map[entity_id].image_path = PARTICLE_TEXTURES[category];
			}
			else
			{
				draw_info_map[entity_id].image_path = PLAYER_PARTICLE_TEXTURES[category];
			}
		}

		for (auto const& [entity_id, orientation_dependent_drawinfo] : orientation_dependent_drawinfo_map)
		{
			if (width_and_height_map[entity_id].width_and_height.x > width_and_height_map[entity_id].width_and_height.y)
			{
				draw_info_map[entity_id].image_path = orientation_dependent_drawinfo.horisontal_image_path;
			}
			else
			{
				draw_info_map[entity_id].image_path = orientation_dependent_drawinfo.vertical_image_path;
			}
		}

		for (auto const& [entity_id, magnetic_field] : magnetic_field_map)
		{
			int category = FindClosest(MAGNETIC_FIELD_STRENGTH_CATEGORIES, magnetic_field.field_strength);
			draw_info_map[entity_id].image_path = MAGNETIC_FIELD_TEXTURES[category];
		}

		for (auto const& [entity_id, electric_field] : electric_field_map)
		{
			int category = FindClosest(ELECTRIC_FIELD_STRENGTH_CATEGORIES, Magnitude(electric_field.field_vector));
			draw_info_map[entity_id].image_path = ELECTRIC_FIELD_TEXTURES[category];
			if (abs(electric_field.field_vector.x) > abs(electric_field.field_vector.y))
			{
				if (electric_field.field_vector.x > 0)
				{
					draw_info_map[entity_id].quarter_turn_rotations = 3;
				}
				else
				{
					draw_info_map[entity_id].quarter_turn_rotations = 1;
				}
			}
			else
			{
				if (electric_field.field_vector.y > 0)
				{
					draw_info_map[entity_id].quarter_turn_rotations = 0;
				}
				else
				{
					draw_info_map[entity_id].quarter_turn_rotations = 2;
				}
			}
		}
	}
};