#pragma once
#include "systems/render_text.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Text.hpp"
#include "components/draw_info.hpp"
#include "components/physics.hpp"
#include "components/text.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"

void RenderTextSystem::Update(Level& level, float dt)
{
	for (auto const& [entity, text, draw_priority, position] : level.GetEntitiesWith<Text, DrawPriority, Position>())
	{
		if (font_.count(text->font_path) == 0)
		{
			font_[text->font_path].loadFromFile(text->font_path);
		}
		if (text_height_[text->font_path].count(text->size) == 0)
		{
			sf::Text alignment_reference = sf::Text("N", font_[text->font_path], text->size);
			text_height_[text->font_path][text->size] = alignment_reference.getLocalBounds().height / 2 + alignment_reference.getLocalBounds().top;
		}
		text_[entity].setString(text->content);
		text_[entity].setFont(font_[text->font_path]);
		text_[entity].setCharacterSize(text->size);
		sf::FloatRect bounds = text_[entity].getLocalBounds();
		text_[entity].setOrigin(bounds.width / 2 + bounds.left, text_height_[text->font_path][text->size]);
		text_[entity].setPosition(position->position);
		text_[entity].setFillColor(text->color);
		text_[entity].setOutlineColor(text->outline_color);
		text_[entity].setOutlineThickness(text->outline_thickness);
		level.drawables[draw_priority->draw_priority].push_back({ &text_[entity], text->apply_shader ? entity : std::optional<Entity>() });
	}
}