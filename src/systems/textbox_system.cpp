#include "components/animated_properties.hpp"
#include "components/button_events.hpp"
#include "components/draw_info.hpp"
#include "components/position.hpp"
#include "components/text.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "utils/string_manip.hpp"
#include <string>
#include <vector>

static void EditString(std::string& s, const std::string& edit)
{
	if (edit == "\b")
	{
		if (s.length() == 0)
		{
			return;
		}
		s.pop_back();
		return;
	}
	s.append(edit);
}

static void EditString(std::string& s, const std::vector<std::string>& edits)
{
	for (const std::string& edit : edits)
	{
		EditString(s, edit);
	}
}

class TextBoxCurser
{};

void TextBoxSystem::Update(Level& level, float dt)
{
	for (auto [entity_id, text, hovered, position] : level.GetEntitiesWith<Text, Hovered, Position>())
	{
		EditString(text->content, cursor_and_keys_.text_input);
		int cursor_id = level.GetSingletonId<TextBoxCurser>([text_size = text->size](ECSScene& scene) {
			int cursor_id = scene.CreateEntityId();
			scene.AddComponent<Position>(cursor_id);
			scene.AddComponent<Text>(cursor_id)->size = text_size;
			scene.AddComponent<DrawPriority>(cursor_id)->draw_priority = UI_BASE_DRAW_PRIORITY;
			AnimatedOpacity* animated_opacity = scene.AddComponent<AnimatedOpacity>(cursor_id);
			animated_opacity->start_time = globals.time;
			animated_opacity->animation_func = [](float t) { return sf::Uint16(255 * ((1 + int(t / (0.530))) % 2)); }; //530ms is the standard blink rate
			return cursor_id;
		});

		level.GetComponent<Text>(cursor_id)->content = RightShiftString("|", text->content.length() + 1);
		level.GetComponent<Position>(cursor_id)->position = position->position + sf::Vector2f(0, -0.06 * text->size);
	}
}