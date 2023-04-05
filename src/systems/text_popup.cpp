#include "_pure_DO_systems.hpp"
#include "level.hpp"
#include "utils/level_id.hpp"
#include "utils/math.hpp"

void TextPopupSystem::Update(Level& level, float dt)
{
	for (auto [entity_id, text_popup_spawner, draw_info] : level.GetEntitiesWith<TextPopupSpawner, DrawInfo>())
	{
		draw_info->image_path = (in_edit_mode_ && !IsMenu(active_level_id_)) ? "content\\textures\\white.png" : "content\\textures\\transparent.png";
	}
	for (auto [entity_id, player, intersection] : level.GetEntitiesWith<Player, Intersection>())
	{
		for (auto& i : intersection->entered_this_frame_ids)
		{
			if (TextPopupSpawner* text_popup_spawner = level.RawGetComponent<TextPopupSpawner>(i))
			{
				auto [popup_id, popup_width_and_height, popup_draw_info, popup_text, popup_draw_priority, popup_position, popup_animated_position] = level.CreateEntityWith<WidthAndHeight, DrawInfo, Text, DrawPriority, Position, AnimatedPosition>();
				popup_draw_info->scale_to_fit = true;
				popup_draw_info->image_path = "content\\textures\\gray.png";
				popup_width_and_height->width_and_height = sf::Vector2f(2400, 240);
				popup_text->content = text_popup_spawner->content;
				popup_text->size = 100;
				popup_draw_priority->draw_priority = UI_BASE_DRAW_PRIORITY;
				popup_animated_position->start_time = globals.time;
				popup_animated_position->animation_func = [&](float t) { return sf::Vector2f(level.GetSize().x / 2, 180 * 2 * (Ease(2 * t - 0.5, 1, 3) - 0.5)); };
				level.AddComponent<ScheduledDelete>(popup_id)->delete_at = globals.time + 5;
			}
		}
	}
}