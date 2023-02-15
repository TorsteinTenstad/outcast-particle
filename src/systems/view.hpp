#pragma once

#include "animation.hpp"
#include "constants.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utils.hpp"

class ViewSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		sf::Vector2f level_size = level.GetSize();
		sf::Vector2f view_center = level_size / 2.f;
		if (level.GetMode() == EDIT_MODE)
		{
			level_size.x += EDIT_MODE_PANEL_WIDTH;
			view_center.x -= EDIT_MODE_PANEL_WIDTH / 2;
		}
		sf::Vector2f view_size = level_size;

		float level_aspect_ratio = level_size.x / level_size.y;
		sf::Vector2u window_size = globals.render_window.getSize();
		float window_aspect_ratio = ((float)window_size.x) / window_size.y;
		if (window_aspect_ratio > level_aspect_ratio)
		{
			view_size.x = view_size.x * window_aspect_ratio / level_aspect_ratio;
		}
		else
		{
			view_size.y = view_size.y / (window_aspect_ratio / level_aspect_ratio);
		}

		globals.render_window.setView(sf::View(view_center, view_size));
	}
};