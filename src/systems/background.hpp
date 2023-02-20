#pragma once
#include <assert.h>

#include "components/draw_info.hpp"
#include "constants.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include <tuple>

class BackgroundSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		auto entities = level.GetEntitiesWith<Background, DrawInfo, DrawPriority, WidthAndHeight, Position>();
		assert(!(entities.size() > 1));
		if (entities.size() == 0)
		{
			auto entity = level.CreateEntitiyWith<Background, DrawInfo, DrawPriority, WidthAndHeight, Position>();
			level.GetComponent<DrawPriority>(std::get<int>(entity))->draw_priority = BACKGROUND_DRAW_PRIORITY;
		}
		for (auto [entity_id, background, draw_info, draw_priority, width_and_height, position] : entities)
		{
			auto level_size = level.GetSize();
			width_and_height->width_and_height = level_size;
			position->position = level_size / 2.f;
			bool in_edit_mode = level.GetMode() == EDIT_MODE;
			draw_info->image_path = in_edit_mode ? "content\\textures\\grid.png" : "content\\textures\\background.png";
			draw_info->scale_to_fit = !in_edit_mode;
		}
	}
};