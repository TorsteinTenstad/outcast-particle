#pragma once
#include "cursor_and_keys.hpp"
#include "level.hpp"

class GameSystem
{
protected:
	const std::string& active_level_id_;
	const bool& is_in_level_editing_;
	CursorAndKeys& cursor_and_keys_;

public:
	GameSystem(std::string& active_level_id, bool& is_in_level_editing, CursorAndKeys& cursor_and_keys) :
		active_level_id_(active_level_id),
		is_in_level_editing_(is_in_level_editing),
		cursor_and_keys_(cursor_and_keys)
	{}
	virtual ~GameSystem() = default;
	virtual void Update(Level& level, float dt) = 0;
};