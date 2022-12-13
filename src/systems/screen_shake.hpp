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
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		float screen_size_shake = 1;
		Animate(globals.time, screen_size_shake, level.screen_size_shake_animation, FakeSigmoid);

		sf::Vector2f view_size = level.size;
		float window_aspect_ratio = ((float)globals.window_size.x) / globals.window_size.y;
		if (window_aspect_ratio > 16 / 9)
		{
			view_size.x = window_aspect_ratio * view_size.y;
		}
		else
		{
			view_size.y = view_size.x / window_aspect_ratio;
		}

		globals.render_window.setView(sf::View(view_size / 2.f, view_size * screen_size_shake));
	}
};