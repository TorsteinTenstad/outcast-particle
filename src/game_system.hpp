#pragma once
#include "cursor_and_keys.hpp"
#include "level.hpp"
#include "modes.hpp"

class GameSystem
{
protected:
	Mode& mode_;
	CursorAndKeys& cursor_and_keys_;

	Mode previous_mode_ = STARTING_MODE;
	int previous_level_ = STARTING_LEVEL;

public:
	GameSystem(Mode& mode, CursorAndKeys& cursor_and_keys) :
		mode_(mode),
		cursor_and_keys_(cursor_and_keys)
	{}
	virtual ~GameSystem() = default;
	void UpdateGameSystem(Level& level, float dt)
	{
		if (level.id != previous_level_)
		{
			OnExitLevel(level);
			OnEnterLevel(level);
			previous_level_ = level.id;
		}
		if (mode_ != previous_mode_)
		{
			OnEnterMode(level, previous_mode_);
			OnExitMode(level, mode_);
			previous_mode_ = mode_;
		}
		Update(level, dt);
	}

	virtual void Update(Level& level, float dt) = 0;
	virtual void OnEnterMode(Level& level, Mode mode) = 0;
	virtual void OnExitMode(Level& level, Mode mode) = 0;
	virtual void OnEnterLevel(Level& level) = 0;
	virtual void OnExitLevel(Level& level) = 0;
};