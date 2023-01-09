#pragma once

#include "controls_config.hpp"
#include "cursor_and_keys.hpp"
#include "game.hpp"
#include "level.hpp"
#include "level_state.hpp"
#include "modes.hpp"

class ModeSystem : public GameSystem
{
private:
	std::function<void(Mode)> set_mode_;
	std::function<Mode(void)> get_mode_;
	std::function<bool(void)> in_level_;

public:
	using GameSystem::GameSystem;
	void SetGameControlFunctions(std::function<void(Mode)> set_mode, std::function<Mode(void)> get_mode, std::function<bool(void)> in_level)
	{
		set_mode_ = set_mode;
		get_mode_ = get_mode;
		in_level_ = in_level;
	}
	void Update(Level& level, float dt)
	{
		if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.EDIT_MODE])
		{
			if (get_mode_() == PLAY_MODE && level.editable)
			{
				Request_mode(EDIT_MODE);
			}
			else if (get_mode_() == EDIT_MODE)
			{
				Request_mode(PLAY_MODE);
			}
		}
		if (cursor_and_keys_.key_released_this_frame[globals.key_config.MENU])
		{
			if (get_mode_() == PLAY_MODE)
			{
				cursor_and_keys_.key_released_this_frame[globals.key_config.MENU] = false; //probably a bad idea, but it works for now

				Request_mode(PAUSE_MODE);
			}
		}
		if (!globals.render_window.hasFocus() && get_mode_() == PLAY_MODE)
		{
			Request_mode(PAUSE_MODE);
		}
		if (ComputeState(level) == WON)
		{
			Request_mode(LEVEL_COMPLETED_MODE);
		}
		else if (ComputeState(level) == FAILED)
		{
			Request_mode(LEVEL_FAILED_MODE);
		}
	}
	void Request_mode(Mode requested_mode)
	{
		if (requested_mode == PLAY_MODE)
		{
			set_mode_(PLAY_MODE);
		}
		if (requested_mode == EDIT_MODE)
		{
			set_mode_(EDIT_MODE);
		}
		if (requested_mode == PAUSE_MODE && in_level_())
		{
			set_mode_(PAUSE_MODE);
		}
		if (requested_mode == LEVEL_FAILED_MODE && in_level_())
		{
			set_mode_(LEVEL_FAILED_MODE);
		}
		if (requested_mode == LEVEL_COMPLETED_MODE && in_level_())
		{
			set_mode_(LEVEL_COMPLETED_MODE);
		}
	}
	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};