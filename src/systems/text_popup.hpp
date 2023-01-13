#pragma once

#include "game_system.hpp"
#include "level.hpp"

class TextPopupSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		for (auto [entity_id, player, intersection] : level.GetEntitiesWith<Player, Intersection>())
		{
			for (auto& i : intersection->entered_this_frame_ids)
			{
				if (level.GetComponent<TextPopupSpawner>().count(i) > 0)
				{
					int popup_id = level.CreateEntityId();
					std::cout << level.GetComponent<TextPopupSpawner>()[i].content << std::endl;
					level.GetComponent<Text>()[popup_id].content = level.GetComponent<TextPopupSpawner>()[i].content;
					level.GetComponent<DrawPriority>()[popup_id].draw_priority = UI_BASE_DRAW_PRIORITY;
					level.GetComponent<Position>()[popup_id];
					level.GetComponent<AnimatedPosition>()[popup_id].start_time = globals.time;
					level.GetComponent<AnimatedPosition>()[popup_id].position_at_time_func = [=](float t) { return sf::Vector2f(level.size.x / 2, level.size.y / 2 + 200 * t); };
				}
			}
		}
	}
	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};