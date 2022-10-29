#pragma once
#include "level.hpp"

class Game
{
private:
	std::vector<Level> levels_;

public:
	Game();
	void Update(float dt);
};