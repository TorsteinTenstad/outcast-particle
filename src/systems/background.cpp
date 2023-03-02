#include "_pure_DO_systems.hpp"

void BackgroundSystem::Update(Level& level, float dt)
{
	auto entity = level.GetEntitiesWith<Background, DrawInfo, DrawPriority, WidthAndHeight, Position>();
	assert(!(entity.size() > 1));
	if (entity.size() == 0)
	{
		entity.push_back(level.CreateEntitiyWith<Background, DrawInfo, DrawPriority, WidthAndHeight, Position>());
		level.GetComponent<DrawPriority>(std::get<int>(entity[0]))->draw_priority = BACKGROUND_DRAW_PRIORITY;
	}
	for (auto [entity_id, background, draw_info, draw_priority, width_and_height, position] : entity)
	{
		auto level_size = level.GetSize();
		width_and_height->width_and_height = level_size;
		position->position = level_size / 2.f;
		bool in_edit_mode = level.GetMode() == EDIT_MODE;
		draw_info->image_path = in_edit_mode ? "content\\textures\\grid.png" : "content\\textures\\background.png";
		draw_info->scale_to_fit = !in_edit_mode;
	}
}