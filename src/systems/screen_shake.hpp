#pragma once
#include "PCH.hpp"
#include "animation.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utils.hpp"

class ScreenShakeSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		float screen_size_shake = 1;
		Animate(globals.time, screen_size_shake, level.screen_size_shake_animation, FakeSigmoid);

		sf::Vector2f view_size = level.size;
		float level_aspect_ratio = ((float)level.size.x) / level.size.y;
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

		globals.render_window.setView(sf::View(level.size / 2.f, view_size * screen_size_shake));
	}
};