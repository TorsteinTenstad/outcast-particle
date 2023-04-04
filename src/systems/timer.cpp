#pragma once
#include "entity_creation.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "utils/level_id.hpp"
#include "utils/string_manip.hpp"
#include "utils/string_parsing.hpp"
#include <iomanip>
#include <sstream>

void TimerSystem::Update(Level& level, float dt)
{
	if (!globals.general_config.show_timer || IsMenu(active_level_id_))
	{
		return;
	}

	sf::Vector2f level_size = level.GetSize();
	auto [id, timer_button] = level.GetSingletonIncludeID<TimerButton>([level_size](ECSScene& level, int entity_id) {
		level.AddComponent<DrawPriority>(entity_id)->draw_priority = 100;
		level.AddComponent<DrawInfo>(entity_id, { "content\\textures\\white.png", false, 0 });
		level.AddComponent<Shader>(entity_id, { "", "shaders\\round_corners.frag", {}, {}, {} });
		level.AddComponent<Position>(entity_id)->position = sf::Vector2f(level_size.x - 2.5 * BLOCK_SIZE, 0.5 * BLOCK_SIZE);
		level.AddComponent<WidthAndHeight>(entity_id)->width_and_height = sf::Vector2f(5 * BLOCK_SIZE, 1 * BLOCK_SIZE);
		level.AddComponent<FillColor>(entity_id)->color.a = 50;
		level.AddComponent<Text>(entity_id);
	});
	level.GetComponent<Text>(id)->content = FloatToStringWithPrecision(level.GetSingleton<LevelCompletionTimer>()->duration, 3);
}