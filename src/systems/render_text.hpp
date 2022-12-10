#pragma once
#include "PCH.hpp"
#include "components/draw_info.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"

class RenderTextSystem : public GameSystem
{
private:
	sf::Font font_;
	sf::Text sample_text_;
	std::map<int, sf::Text> text_;
	std::map<unsigned int, float> text_height_;

public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		auto& draw_priority_map = level.GetComponent<DrawPriority>();
		auto& position_map = level.GetComponent<Position>();
		auto& text_map = level.GetComponent<Text>();

		font_.loadFromFile("content\\Roboto-Medium.ttf");
		sample_text_.setString("N");
		sample_text_.setFont(font_);
		for (auto const& [entity_id, entity_text] : text_map)
		{
			assert(draw_priority_map.count(entity_id) > 0);
			if (text_height_.count(entity_text.size) == 0)
			{
				sample_text_.setCharacterSize(entity_text.size);
				text_height_[entity_text.size] = sample_text_.getLocalBounds().height / 2 + sample_text_.getLocalBounds().top;
			};
			text_[entity_id].setString(entity_text.content);
			text_[entity_id].setFont(font_);
			text_[entity_id].setCharacterSize(entity_text.size);
			sf::FloatRect bounds = text_[entity_id].getLocalBounds();
			text_[entity_id].setOrigin(bounds.width / 2 + bounds.left, text_height_[entity_text.size]);
			text_[entity_id].setPosition(position_map[entity_id].position);
			level.drawables[draw_priority_map[entity_id].draw_priority].push_back({ entity_id, &text_[entity_id] });
		}
	}
};