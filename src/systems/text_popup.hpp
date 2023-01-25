#pragma once

#include "game_system.hpp"
#include "level.hpp"
#include "utils.hpp"

class TextPopupSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		for (auto [entity_id, text_popup_spawner, draw_info] : level.GetEntitiesWith<TextPopupSpawner, DrawInfo>())
		{
			draw_info->image_path = level.GetMode() == EDIT_MODE ? "content\\textures\\white.png" : "content\\textures\\transparent.png";
		}
		for (auto [entity_id, player, intersection] : level.GetEntitiesWith<Player, Intersection>())
		{
			for (auto& i : intersection->entered_this_frame_ids)
			{
				if (level.HasComponents<TextPopupSpawner>(i))
				{
					auto [popup_id, popup_width_and_height, popup_draw_info, popup_text, popup_draw_priority, popup_position, popup_animated_position] = level.CreateEntitiyWith<WidthAndHeight, DrawInfo, Text, DrawPriority, Position, AnimatedPosition>();
					popup_draw_info->scale_to_fit = true;
					popup_draw_info->image_path = "content\\textures\\gray.png";
					popup_width_and_height->width_and_height = sf::Vector2f(2400, 240);
					popup_text->content = level.GetComponent<TextPopupSpawner>()[i].content;
					popup_text->size = 100;
					popup_draw_priority->draw_priority = UI_BASE_DRAW_PRIORITY;
					popup_animated_position->start_time = globals.time;
					popup_animated_position->position_at_time_func = [&](float t) { return sf::Vector2f(level.size.x / 2, 180 * (Ease(2 * t - 0.5, 1, 3)) - 0.5); };
					level.AddComponent<ScheduledDelete>(popup_id)->delete_at = globals.time + 5;
				}
			}
		}
	}
};