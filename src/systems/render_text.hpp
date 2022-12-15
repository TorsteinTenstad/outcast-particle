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
	std::map<int, sf::Text> text_;
	std::map<std::string, std::map<unsigned int, float>> text_height_;
	std::map<std::string, sf::Font> font_;

public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		auto& draw_priority_map = level.GetComponent<DrawPriority>();
		auto& position_map = level.GetComponent<Position>();
		auto& text_map = level.GetComponent<Text>();

		for (auto const& [entity_id, entity_text] : text_map)
		{
			assert(draw_priority_map.count(entity_id) > 0);
			if (font_.count(entity_text.font_path) == 0)
			{
				font_[entity_text.font_path].loadFromFile(entity_text.font_path);
			}
			if (text_height_[entity_text.font_path].count(entity_text.size) == 0)
			{
				sf::Text allignment_refrence = sf::Text("N", font_[entity_text.font_path], entity_text.size);
				text_height_[entity_text.font_path][entity_text.size] = allignment_refrence.getLocalBounds().height / 2 + allignment_refrence.getLocalBounds().top;
			}
			text_[entity_id].setString(entity_text.content);
			text_[entity_id].setFont(font_[entity_text.font_path]);
			text_[entity_id].setCharacterSize(entity_text.size);
			sf::FloatRect bounds = text_[entity_id].getLocalBounds();
			text_[entity_id].setOrigin(bounds.width / 2 + bounds.left, text_height_[entity_text.font_path][entity_text.size]);
			text_[entity_id].setPosition(position_map[entity_id].position);
			level.drawables[draw_priority_map[entity_id].draw_priority].push_back({ entity_id, &text_[entity_id] });
		}
	}
};