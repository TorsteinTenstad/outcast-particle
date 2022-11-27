#pragma once
#include "PCH.hpp"
#include "components/draw_info.hpp"
#include "constants.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include "utilityfunctions.hpp"

class SetDrawInfoSystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		(void)cursor_and_keys;
		(void)dt;
		std::map<int, DrawInfo>& draw_info_map = level.GetComponent<DrawInfo>();
		std::map<int, ChargeDependentDrawInfo>& charge_dependent_drawinfo_map = level.GetComponent<ChargeDependentDrawInfo>();
		std::map<int, OrientationDependentDrawInfo>& orientation_dependent_drawinfo_map = level.GetComponent<OrientationDependentDrawInfo>();
		std::map<int, Charge>& charge_map = level.GetComponent<Charge>();
		std::map<int, WidthAndHeight>& width_and_height_map = level.GetComponent<WidthAndHeight>();
		std::map<int, ElectricField>& electric_field_map = level.GetComponent<ElectricField>();
		std::map<int, MagneticField>& magnetic_field_map = level.GetComponent<MagneticField>();
		std::map<int, Player>& player_map = level.GetComponent<Player>();

		for (auto const& [entity_id, charge_dependent_drawinfo] : charge_dependent_drawinfo_map)
		{
			int charge_category = FindClosest(PARTICLE_CHARGE_CATEGORIES, charge_map[entity_id].charge);
			if (player_map.count(entity_id))
			{
				draw_info_map[entity_id].image_path = PARTICLE_TEXTURES[charge_category];
			}
			else
			{
				draw_info_map[entity_id].image_path = PLAYER_PARTICLE_TEXTURES[charge_category];
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
			if (magnetic_field.field_strength > 0)
			{
			}
		}

		for (auto const& [entity_id, electric_field] : electric_field_map)
		{
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