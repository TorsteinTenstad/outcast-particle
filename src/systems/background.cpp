#include "_pure_DO_systems.hpp"

void BackgroundSystem::Update(Level& level, float dt)
{
	auto backgrounds = level.GetEntitiesWith<Background, DrawInfo, DrawPriority, WidthAndHeight, Position>();
	if (backgrounds.size() == 0)
	{
		backgrounds.push_back(level.CreateEntitiyWith<Background, DrawInfo, DrawPriority, WidthAndHeight, Position>());
		level.GetComponent<DrawPriority>(std::get<int>(backgrounds.back()))->draw_priority = BACKGROUND_DRAW_PRIORITY;
		backgrounds.push_back(level.CreateEntitiyWith<Background, DrawInfo, DrawPriority, WidthAndHeight, Position>());
		level.GetComponent<DrawPriority>(std::get<int>(backgrounds.back()))->draw_priority = BACKGROUND_DRAW_PRIORITY;
	}
	assert(backgrounds.size() == 2);
	int main_background = 0;
	for (auto [backgrounds_id, background, draw_info, draw_priority, width_and_height, position] : backgrounds)
	{
		auto level_size = level.GetSize();
		bool in_edit_mode = level.GetMode() == EDIT_MODE;
		draw_info->image_path = in_edit_mode ? "content\\textures\\grid.png" : "content\\textures\\background.png";
		draw_info->image_path = main_background ? draw_info->image_path : "content\\textures\\ui_background.png";
		width_and_height->width_and_height = main_background ? level_size : level_size + level.ui_bars_size;
		position->position = main_background ? level_size / 2.f : level_size / 2.f - level.ui_bars_size / 2.f;
		draw_info->scale_to_fit = !in_edit_mode;
		main_background++;
	}
}