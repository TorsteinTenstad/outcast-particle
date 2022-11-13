#pragma once
#include "PCH.hpp"
#include "components/draw_info.hpp"
#include "game_system.hpp"
#include "level.hpp"

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

		for (auto const& [entity_id, charge_dependent_drawinfo] : charge_dependent_drawinfo_map)
		{
			if (charge_map[entity_id].charge > 0)
			{
				draw_info_map[entity_id].image_path = charge_dependent_drawinfo.positive_charge_image_path;
			}
			else if (charge_map[entity_id].charge < 0)
			{
				draw_info_map[entity_id].image_path = charge_dependent_drawinfo.negative_charge_image_path;
			}
			else
			{
				draw_info_map[entity_id].image_path = charge_dependent_drawinfo.neutral_charge_image_path;
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
	}
};