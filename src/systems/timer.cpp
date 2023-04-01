#pragma once
#include "_pure_DO_systems.hpp"
#include "utils/string_parsing.hpp"
#include <iomanip>
#include <sstream>

void TimerSystem::Update(Level& level, float dt)
{
	std::stringstream current_duration;
	current_duration << std::fixed << std::setprecision(3);
	current_duration << level.GetSingleton<LevelCompletionTimer>()->duration;

	for (auto [entity_id, timer_button, text] : level.GetEntitiesWith<TimerButton, Text>())
	{
		text->content = current_duration.str();
	}
}
