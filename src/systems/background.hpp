#pragma once
#include "PCH.hpp"
#include "components/draw_info.hpp"
#include "constants.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "modes.hpp"

class BackgroundSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		auto& background_map = level.GetComponent<Background>();
		auto& draw_priority_map = level.GetComponent<DrawPriority>();
		auto& draw_info_map = level.GetComponent<DrawInfo>();
		auto& position_map = level.GetComponent<Position>();
		auto& width_and_height_map = level.GetComponent<WidthAndHeight>();

		assert(!(background_map.size() > 1));
		if (background_map.size() == 0)
		{
			int entity_id = level.CreateEntityId();
			background_map[entity_id];
			draw_priority_map[entity_id].draw_priority = BACKGROUND_DRAW_PRIORITY;
		}
		int entity_id = background_map.begin()->first;
		width_and_height_map[entity_id].width_and_height = level.size;
		position_map[entity_id].position = level.size / 2.f;
		if (mode_ == EDIT_MODE)
		{
			draw_info_map[entity_id].image_path = "content\\textures\\grid.png";
			draw_info_map[entity_id].scale_to_fit = false;
		}
		else
		{
			draw_info_map[entity_id].image_path = "content\\textures\\background.png";
			draw_info_map[entity_id].scale_to_fit = true;
		}
	}
	void OnEnterMode(Level& level, Mode mode) {};
	void OnExitMode(Level& level, Mode mode) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};