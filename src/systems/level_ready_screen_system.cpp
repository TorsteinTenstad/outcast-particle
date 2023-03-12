#include "SFML/Config.hpp"
#include "_pure_DO_systems.hpp"
#include "utils.hpp"
#include <cmath>

void LevelReadyScreenSystem::Update(Level& level, float dt)
{
	if (level.GetMode() != READY_MODE)
	{
		return;
	}

	sf::Vector2f level_size = level.GetSize();
	sf::Vector2u level_grid_size = level.GetGridSize();

	level.GetSingleton<ReadyScreen>([&level_size, &level_grid_size](ESCScene& level, int entity_id) {
		level.AddComponent<DrawInfo>(entity_id)->image_path = "content\\textures\\transparent.png";
		level.AddComponent<DrawPriority>(entity_id)->draw_priority = UI_BASE_DRAW_PRIORITY + 5;
		level.AddComponent<Position>(entity_id)->position = level_size / 2.f;
		level.AddComponent<WidthAndHeight>(entity_id)->width_and_height = sf::Vector2f(0.75 * level_size.x, (3.f / 18.f) * level_size.y);
		level.AddComponent<FillColor>(entity_id);
		Text* text = level.AddComponent<Text>(entity_id);
		text->content = "Press any key to start";
		text->size = 200 * float(level_grid_size.y) / 16.f;
		text->outline_color = sf::Color::Black;
		text->outline_thickness = 10 * float(level_grid_size.y) / 16.f;
		AnimatedOpacity* animated_opacity = level.AddComponent<AnimatedOpacity>(entity_id);
		animated_opacity->animation_func = [](float t) { return sf::Uint8(255 * Smoothstep(0.5 * std::cos(PI * t) + 0.5)); };
		animated_opacity->start_time = globals.time;
	});

	auto conditionaly_enter_play_mode = [&level](bool enter) {
		if (enter)
		{
			level.SetMode(PLAY_MODE);
			level.DeleteEntitiesWith<ReadyScreen>();
		}
	};

	conditionaly_enter_play_mode(cursor_and_keys_.mouse_button_pressed_this_frame[sf::Mouse::Left]);
	for (auto [key, pressed_this_frame] : cursor_and_keys_.key_pressed_this_frame)
	{
		conditionaly_enter_play_mode(pressed_this_frame);
	}
}
