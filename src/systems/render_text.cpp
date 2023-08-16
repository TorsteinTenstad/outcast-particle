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

sf::Vector2f RenderTextSystem::GetOrigin(TextOrigin text_origin, sf::FloatRect bounds, float reference_height)
{
	switch (text_origin)
	{
		case TextOrigin::TOP_LEFT:
			return sf::Vector2f(bounds.left, bounds.top);
		case TextOrigin::TOP_RIGHT:
			return sf::Vector2f(bounds.left + bounds.width, bounds.top);
		case TextOrigin::TOP_CENTER:
			return sf::Vector2f(bounds.left + bounds.width / 2, bounds.top);
		case TextOrigin::CENTER_LEFT:
			return sf::Vector2f(bounds.left, bounds.top + bounds.height / 2);
		case TextOrigin::CENTER_RIGHT:
			return sf::Vector2f(bounds.left + bounds.width, bounds.top + bounds.height / 2);
		case TextOrigin::CENTER_CENTER:
			return sf::Vector2f(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
		case TextOrigin::BOTTOM_LEFT:
			return sf::Vector2f(bounds.left, bounds.top + bounds.height);
		case TextOrigin::BOTTOM_RIGHT:
			return sf::Vector2f(bounds.left + bounds.width, bounds.top + bounds.height);
		case TextOrigin::BOTTOM_CENTER:
			return sf::Vector2f(bounds.left + bounds.width / 2, bounds.top + bounds.height);
		case TextOrigin::REFERENCE_HEIGHT_LEFT:
			return sf::Vector2f(bounds.left, reference_height);
		case TextOrigin::REFERENCE_HEIGHT_RIGHT:
			return sf::Vector2f(bounds.left + bounds.width, reference_height);
		case TextOrigin::REFERENCE_HEIGHT_CENTER:
			return sf::Vector2f(bounds.left + bounds.width / 2, reference_height);

		default:
			break;
	}
	assert(false);
	return sf::Vector2f(0, 0);
}

void RenderTextSystem::Update(Level& level, float dt)
{
	for (auto const& [entity, text, draw_priority, position] : level.GetEntitiesWith<Text, DrawPriority, Position>())
	{
		if (font_.count(text->font_path) == 0)
		{
			if (!font_[text->font_path].loadFromFile(text->font_path))
			{
				globals.errors += Error(ErrorNumber::LOAD_FONT, "Failed to load font from\n" + text->font_path);
			}
		}
		if (text_height_[text->font_path].count(text->size) == 0)
		{
			sf::Text alignment_reference = sf::Text("0N", font_[text->font_path], text->size);
			text_height_[text->font_path][text->size] = alignment_reference.getLocalBounds().height / 2 + alignment_reference.getLocalBounds().top;
		}
		text_[entity].setString(text->content);
		text_[entity].setFont(font_[text->font_path]);
		text_[entity].setCharacterSize(text->size);
		sf::FloatRect bounds = text_[entity].getLocalBounds();
		text_[entity].setOrigin(GetOrigin(text->origin, bounds, text_height_[text->font_path][text->size]));
		text_[entity].setPosition(position->position);
		text_[entity].setFillColor(text->color);
		text_[entity].setOutlineColor(text->outline_color);
		text_[entity].setOutlineThickness(text->outline_thickness);
		level.drawables[draw_priority->draw_priority].push_back({ &text_[entity], text->apply_shader ? entity : std::optional<Entity>() });
	}
}