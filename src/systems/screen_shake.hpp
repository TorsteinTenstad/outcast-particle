#pragma once
#include "PCH.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utilityfunctions.hpp"
#include "animation.hpp"

class ScreenShakeSystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		float screen_size_shake = 1;
		Animate(globals.time, screen_size_shake, level.screen_size_shake_animation, FakeSigmoid);
		globals.render_window.setView(sf::View(level.size / 2.f, level.size * screen_size_shake));
	}
};