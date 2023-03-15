#include "_pure_DO_systems.hpp"
#include "constants.hpp"
#include "globals.hpp"
#include "level.hpp"


void ViewSystem::Update(Level& level, float dt)
{
	sf::Vector2f level_size = level.GetSize();
	sf::Vector2f ui_inclusive_level_size = level_size + level.ui_bars_size;
	sf::Vector2f view_center = level_size / 2.f - level.ui_bars_size / 2.f;
	float level_aspect_ratio = ui_inclusive_level_size.x / ui_inclusive_level_size.y;
	sf::Vector2u window_size = globals.render_window.getSize();
	float window_aspect_ratio = ((float)window_size.x) / window_size.y;

	sf::Vector2f view_size = ui_inclusive_level_size;
	if (window_aspect_ratio > level_aspect_ratio)
	{
		view_size.x *= window_aspect_ratio / level_aspect_ratio;
	}
	else
	{
		view_size.y /= window_aspect_ratio / level_aspect_ratio;
	}

	globals.render_window.setView(sf::View(view_center, view_size));
}