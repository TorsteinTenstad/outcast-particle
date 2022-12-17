#pragma once

class Text
{
public:
	std::string content;
	unsigned size = 200;
	sf::Color color = sf::Color::White;
	std::string font_path = "content\\Roboto-Medium.ttf";
};