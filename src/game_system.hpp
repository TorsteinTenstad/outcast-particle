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
			OnExitLevel(previous_level_);
			OnEnterLevel(level.id);
			previous_level_ = level.id;
		}
		if (mode_ != previous_mode_)
		{
			OnEnterMode(previous_mode_);
			OnExitMode(mode_);
			previous_mode_ = mode_;
		}
		Update(level, dt);
	}

	virtual void Update(Level& level, float dt) = 0;
	virtual void OnEnterMode(Mode mode) = 0;
	virtual void OnExitMode(Mode mode) = 0;
	virtual void OnEnterLevel(int level_id) = 0;
	virtual void OnExitLevel(int level_id) = 0;
};