#pragma once
#include "PCH.hpp"
#include "controls_config.hpp"
#include "cursor_and_keys.hpp"
#include "game.hpp"
#include "level.hpp"

class PauseMenuSystem
{
private:
	std::function<void(int)> set_level_;

public:
	PauseMenuSystem()
	{}
	PauseMenuSystem(std::function<void(int)> set_level)
	{
		set_level_ = set_level;
	}
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		if (cursor_and_keys.key_pressed_this_frame[globals.key_config.MENU])
		{
			set_level_(-1);
		}
	}
};