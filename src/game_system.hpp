#pragma once
#include "cursor_and_keys.hpp"
#include "level.hpp"
#include "modes.hpp"

class GameSystem
{
protected:
	const int& level_id_;
	Mode& mode_;
	CursorAndKeys& cursor_and_keys_;

public:
	GameSystem(int& level_id, Mode& mode, CursorAndKeys& cursor_and_keys) :
		level_id_(level_id),
		mode_(mode),
		cursor_and_keys_(cursor_and_keys)
	{}
	virtual ~GameSystem() = default;
	virtual void Update(Level& level, float dt) = 0;
	virtual void OnEnterMode(Level& level) = 0;
	virtual void OnExitMode(Level& level) = 0;
	virtual void OnEnterLevel(Level& level) = 0;
	virtual void OnExitLevel(Level& level) = 0;
};