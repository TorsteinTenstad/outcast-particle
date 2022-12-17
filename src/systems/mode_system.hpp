#pragma once
#include "PCH.hpp"
#include "controls_config.hpp"
#include "cursor_and_keys.hpp"
#include "game.hpp"
#include "level.hpp"
#include "modes.hpp"

class ModeSystem
{
private:
	std::function<void(Mode)> set_mode_;
	std::function<Mode(void)> get_mode_;
	std::function<bool(void)> in_level_;

public:
	ModeSystem()
	{}
	ModeSystem(std::function<void(Mode)> set_mode, std::function<Mode(void)> get_mode, std::function<bool(void)> in_level)
	{
		set_mode_ = set_mode;
		get_mode_ = get_mode;
		in_level_ = in_level;
	}
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		auto& goal_map = level.GetComponent<Goal>();
		auto& player_map = level.GetComponent<Player>();

		if (cursor_and_keys.key_pressed_this_frame[globals.key_config.EDIT_MODE])
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
		if (cursor_and_keys.key_pressed_this_frame[globals.key_config.MENU])
		{
			if (get_mode_() == PLAY_MODE)
			{
				Request_mode(PAUSE_MODE);
			}
			else if (get_mode_() == PAUSE_MODE)
			{
				Request_mode(PLAY_MODE);
			}
		}
		if (!globals.render_window.hasFocus() && get_mode_() == PLAY_MODE)
		{
			Request_mode(PAUSE_MODE);
		}
		if (player_map.size() == 0 && get_mode_() == PLAY_MODE)
		{
			bool is_goal = false;
			for (auto& [entity_id, goal] : goal_map)
			{
				if (goal.is_goal)
				{
					is_goal = true;
				}
			}
			if (is_goal)
			{
				Request_mode(LEVEL_COMPLETED_MODE);
			}
			else
			{
				Request_mode(LEVEL_FAILED_MODE);
			}
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
	void SetBackground(std::string image_path, bool scale_to_fit)
	{
	}
};