#pragma once
#include "cursor_and_keys.hpp"
#include "level.hpp"

class GameSystem
{
protected:
	const std::string& active_level_id_;
	const bool& in_level_creator_;
	const bool& in_edit_mode_;
	CursorAndKeys& cursor_and_keys_;

public:
	GameSystem(std::string& active_level_id, const bool& in_level_creator, const bool& in_edit_mode, CursorAndKeys& cursor_and_keys) :
		active_level_id_(active_level_id),
		in_level_creator_(in_level_creator),
		in_edit_mode_(in_edit_mode),
		cursor_and_keys_(cursor_and_keys)
	{}
	virtual ~GameSystem() = default;
	virtual void Update(Level& level, float dt) = 0;
};