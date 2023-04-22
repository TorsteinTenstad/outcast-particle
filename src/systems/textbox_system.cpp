#include "components/button_events.hpp"
#include "components/text.hpp"
#include "systems/_pure_DO_systems.hpp"
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

void TextBoxSystem::Update(Level& level, float dt)
{
	for (auto [entity_id, text, hovered] : level.GetEntitiesWith<Text, Hovered>())
	{
		EditString(text->content, cursor_and_keys_.text_input);
	}
}