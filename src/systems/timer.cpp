#pragma once
#include "components/draw_info.hpp"
#include "components/position.hpp"
#include "components/size.hpp"
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
	float level_scale = level.GetScale();
	Entity entity = level.GetSingletonId<TimerButton>([level_size, level_scale](ECSScene& level) {
		Entity entity = level.CreateEntity();
		level.AddComponent<DrawPriority>(entity)->draw_priority = UI_BASE_DRAW_PRIORITY;
		float margin = 20;
		level.AddComponent<Position>(entity)->position = sf::Vector2f(level_size.x - margin, margin);
		auto text_component = level.AddComponent<Text>(entity);
		text_component->font_path = "game_data\\digits_mono.otf";
		text_component->origin = TextOrigin::TOP_RIGHT;
		text_component->size *= level_scale;
		return entity;
	});
	level.GetComponent<Text>(entity)->content = CreateBadgeText(level.GetSingleton<LevelCompletionTimer>()->duration, 2 + globals.general_config.display_precise_badge_time);
}