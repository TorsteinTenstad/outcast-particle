#pragma once
#include "PCH.hpp"
#include "controls_config.hpp"
#include "cursor_and_keys.hpp"
#include "game.hpp"
#include "level.hpp"
#include "modes.hpp"

class PauseMenuSystem
{
private:
	std::function<void(int)> set_level_;
	std::function<void(Mode)> set_mode_;
	std::function<Mode(void)> get_mode_;

public:
	PauseMenuSystem()
	{}
	PauseMenuSystem(std::function<void(int)> set_level, std::function<void(Mode)> set_mode, std::function<Mode(void)> get_mode)
	{
		set_level_ = set_level;
		set_mode_ = set_mode;
		get_mode_ = get_mode;
	}
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		if (cursor_and_keys.key_pressed_this_frame[globals.key_config.MENU])
		{
			set_level_(MAIN_MENU);
		}
		if (cursor_and_keys.key_pressed_this_frame[globals.key_config.EDIT_MODE])
		{
			if (get_mode_() == PLAY_MODE && level.editable)
			{
				set_mode_(EDIT_MODE);
			}
			else if (get_mode_() == EDIT_MODE)
			{
				set_mode_(PLAY_MODE);
			}
		}
		if (cursor_and_keys.key_pressed_this_frame[sf::Keyboard::T])
		{
			if (get_mode_() == PLAY_MODE)
			{
				set_mode_(PAUSE_MODE);
			}
			else if (get_mode_() == PAUSE_MODE)
			{
				set_mode_(PLAY_MODE);
			}
		}
	}
};