#pragma once
#include "SFML/Graphics/Color.hpp"
#include <string>

class TextBox
{
public:
	std::string illegal_characters = "\"*/:<>?\\|_\r\n";
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
};