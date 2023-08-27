#pragma once
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"
#include <optional>
#include <string>

class TextBox
{
public:
	std::string illegal_characters = "\"*/:<>?\\|_\r\n";
};

enum class TextOrigin : int
{
	TOP_LEFT,
	TOP_RIGHT,
	TOP_CENTER,
	CENTER_LEFT,
	CENTER_RIGHT,
	CENTER_CENTER,
	BOTTOM_LEFT,
	BOTTOM_RIGHT,
	BOTTOM_CENTER,
	REFERENCE_HEIGHT_LEFT,
	REFERENCE_HEIGHT_RIGHT,
	REFERENCE_HEIGHT_CENTER
};

class Text
{
public:
	std::string content;
	unsigned size = 120;
	sf::Color color = sf::Color::White;
	sf::Color outline_color = sf::Color::White;
	float outline_thickness = 0;
	std::string font_path = "content\\monof55.ttf";
	bool apply_shader = false;
	TextOrigin origin = TextOrigin::REFERENCE_HEIGHT_CENTER;
	bool render = true;
	std::optional<sf::Vector2f> result_origin = std::nullopt;
	std::optional<sf::Vector2f> result_size = std::nullopt;
};