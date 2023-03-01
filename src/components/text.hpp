#pragma once

class Text
{
public:
	std::string content;
	unsigned size = 120;
	sf::Color color = sf::Color::White;
	std::string font_path = "content\\monof55.ttf";
	bool apply_shader = false;
};