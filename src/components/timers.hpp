#pragma once
#include "level_mode.hpp"

class LevelCompletionTimer
{
public:
	float duration;
};

class MenuDelayTimer
{
public:
	float duration = 0;
	LevelMode pevious_mode;
	bool buttons_initialized = false;
};