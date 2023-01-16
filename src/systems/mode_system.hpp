#pragma once

#include "controls_config.hpp"
#include "cursor_and_keys.hpp"
#include "game.hpp"
#include "level.hpp"
#include "level_state.hpp"
#include "modes.hpp"

class ModeSystem : public GameSystem
{

public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.EDIT_MODE])
		{
			if (mode_ == PLAY_MODE && level.editable)
			{
				Request_mode(EDIT_MODE);
			}
			else if (mode_ == EDIT_MODE)
			{
				Request_mode(PLAY_MODE);
			}
		}
		if (cursor_and_keys_.key_released_this_frame[globals.key_config.MENU])
		{
			if (mode_ == PLAY_MODE)
			{
				cursor_and_keys_.key_released_this_frame[globals.key_config.MENU] = false; //probably a bad idea, but it works for now

				Request_mode(PAUSE_MODE);
			}
		}
		if (!globals.render_window.hasFocus() && mode_ == PLAY_MODE)
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
			mode_ = (PLAY_MODE);
		}
		if (requested_mode == EDIT_MODE)
		{
			mode_ = (EDIT_MODE);
		}
		if (requested_mode == PAUSE_MODE && level_id_ >= 0)
		{
			mode_ = (PAUSE_MODE);
		}
		if (requested_mode == LEVEL_FAILED_MODE && level_id_ >= 0)
		{
			mode_ = (LEVEL_FAILED_MODE);
		}
		if (requested_mode == LEVEL_COMPLETED_MODE && level_id_ >= 0)
		{
			mode_ = (LEVEL_COMPLETED_MODE);
		}
	}
	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};