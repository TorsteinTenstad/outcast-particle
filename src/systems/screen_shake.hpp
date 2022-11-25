#pragma once
#include "PCH.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"

class ScreenShakeSystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		
		sf::Vector2f screen_size_shake;
		sf::Vector2f screen_size = level.size + screen_size_shake;
		globals.render_window.setView(sf::View(screen_size / 2.f, screen_size));
	}
};