#include "components/draw_info.hpp"
#include "components/position.hpp"
#include "components/size.hpp"
#include "systems/_pure_DO_systems.hpp"
#include <cassert>

void BackgroundSystem::Update(Level& level, float dt)
{
	auto backgrounds = level.GetEntitiesWith<Background, DrawInfo, DrawPriority, WidthAndHeight, Position>();
	if (backgrounds.size() == 0)
	{
		backgrounds.push_back(level.CreateEntityWith<Background, DrawInfo, DrawPriority, WidthAndHeight, Position>());
		level.GetComponent<DrawPriority>(std::get<Entity>(backgrounds.back()))->draw_priority = BACKGROUND_DRAW_PRIORITY;
		backgrounds.push_back(level.CreateEntityWith<Background, DrawInfo, DrawPriority, WidthAndHeight, Position>());
		level.GetComponent<DrawPriority>(std::get<Entity>(backgrounds.back()))->draw_priority = BACKGROUND_DRAW_PRIORITY;
	}
	assert(backgrounds.size() == 2);
	int is_main_background = 0;
	for (auto [backgrounds_id, background, draw_info, draw_priority, width_and_height, position] : backgrounds)
	{
		auto level_size = level.GetSize();
		bool in_edit_mode = level.GetMode() == EDIT_MODE;
		draw_info->image_path = in_edit_mode ? "TEXTURES_DIR\\grid.png" : "TEXTURES_DIR\\background.png";
		draw_info->image_path = is_main_background ? draw_info->image_path : "TEXTURES_DIR\\background.png";
		width_and_height->width_and_height = is_main_background ? level_size : level_size + level.ui_bars_size;
		position->position = is_main_background ? level_size / 2.f : level_size / 2.f - level.ui_bars_size / 2.f;
		draw_info->tile = in_edit_mode;
		is_main_background++;
	}
}