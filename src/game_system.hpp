#pragma once
#include "cursor_and_keys.hpp"
#include "level.hpp"

class GameSystem
{
private:
public:
	virtual ~GameSystem() noexcept = default;
	virtual void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt) = 0;
};