#include "SFML/Config.hpp"
#include "components/animated_properties.hpp"
#include "components/draw_info.hpp"
#include "components/position.hpp"
#include "components/size.hpp"
#include "components/text.hpp"
#include "components/text_popup.hpp"
#include "draw_info.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "utils/math.hpp"
#include "utils/string_manip.hpp"
#include "utils/string_parsing.hpp"

#include <cmath>

class ReadyScreen
{
public:
};

void LevelReadyScreenSystem::Update(Level& level, float dt)
{
	if (level.GetMode() != READY_MODE) { return; }
	std::string prompt = "Press any key to start";
	for (auto [entity, text_popup_spawner] : level.GetEntitiesWith<TextPopupSpawner>())
	{
		auto content_parts = SplitString(text_popup_spawner->content, "_");
		std::string additional_prompt = content_parts.front() + HumanName(text_popup_spawner->key) + (content_parts.size() > 1 ? content_parts.back() : "");
		int pad = prompt.size() - additional_prompt.size();
		if (pad > 0)
		{
			additional_prompt = LeftPad(additional_prompt, additional_prompt.size() + pad / 2);
		}
		else if (pad < 0)
		{
			prompt = LeftPad(prompt, prompt.size() - pad / 2);
		}
		prompt += "\n" + additional_prompt;
		break;
	}
	if (!globals.general_config.use_ready_mode)
	{
		level.SetMode(PLAY_MODE);
		return;
	}

	sf::Vector2f level_size = level.GetSize();
	float level_scale = level.GetScale();

	level.GetSingleton<ReadyScreen>([&level_size, &level_scale, prompt](ECSScene& level) {
		Entity entity = level.CreateEntity();
		level.AddComponent<DrawInfo>(entity)->image_path = (TEXTURES_DIR / "transparent.png").string();
		level.AddComponent<DrawPriority>(entity)->draw_priority = UI_BASE_DRAW_PRIORITY + 5;
		level.AddComponent<Position>(entity)->position = level_size / 2.f;
		level.AddComponent<WidthAndHeight>(entity)->width_and_height = sf::Vector2f(0.75 * level_size.x, (3.f / 18.f) * level_size.y);
		level.AddComponent<FillColor>(entity);
		Text* text = level.AddComponent<Text>(entity);
		text->content = prompt;
		text->origin = TextOrigin::CENTER_CENTER;
		text->outline_color = sf::Color::Black;
		text->size = 200 * level_scale;
		text->outline_thickness = 10 * level_scale;
		AnimatedOpacity* animated_opacity = level.AddComponent<AnimatedOpacity>(entity);
		animated_opacity->animation_func = [](float t) { return sf::Uint8(255 * Smoothstep(0.5 * std::cos(PI * t) + 0.5)); };
		animated_opacity->start_time = globals.time;
		return entity;
	});

	auto conditionally_enter_play_mode = [&level](bool enter) {
		if (enter)
		{
			level.SetMode(PLAY_MODE);
			level.DeleteEntitiesWith<ReadyScreen>();
		}
	};

	conditionally_enter_play_mode(cursor_and_keys_.mouse_button_pressed_this_frame[sf::Mouse::Left]);
	for (auto [key, pressed_this_frame] : cursor_and_keys_.key_pressed_this_frame)
	{
		conditionally_enter_play_mode(pressed_this_frame);
		if (pressed_this_frame && key == globals.key_config.PLAYER_SWITCH_CHARGE)
		{
			cursor_and_keys_.key_pressed_this_frame[globals.key_config.PLAYER_SWITCH_CHARGE] = false;
		}
	}
}
