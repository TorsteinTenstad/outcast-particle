#pragma once
#include "components/text.hpp"
#include "components/timers.hpp"
#include "entity_creation.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "utils/level_id.hpp"
#include "utils/string_manip.hpp"
#include "utils/string_parsing.hpp"
#include <iomanip>
#include <sstream>

class TimerButton
{};

void TimerSystem::Update(Level& level, float dt)
{
	if (!globals.general_config.show_timer || IsMenu(active_level_id_))
	{
		return;
	}

	sf::Vector2f level_size = level.GetSize();
	int entity_id = level.GetSingletonId<TimerButton>([level_size](ECSScene& level) {
		auto [entity_id, _] = CreateTimerButton(level, sf::Vector2f(level_size.x - 2.5 * BLOCK_SIZE, 0.5 * BLOCK_SIZE));
		return entity_id;
	});
	level.GetComponent<Text>(entity_id)->content = CreateBadgeText(level.GetSingleton<LevelCompletionTimer>()->duration, 2 + globals.general_config.display_precise_badge_time);
}