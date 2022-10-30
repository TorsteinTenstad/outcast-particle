#pragma once
#include "cursor_and_keys.hpp"
#include "level.hpp"
#include "sys_sfml_event.hpp"

class Game
{
private:
	std::vector<Level> levels_;

	SFMLEventSystem event_system_;
	CursorAndKeys cursor_and_keys_;

public:
	Game();
	void Update(float dt);
};