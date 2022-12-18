#pragma once

class Button
{
public:
	sf::Keyboard::Key equivalent_key = sf::Keyboard::Unknown;
	std::function<void(void)> on_click;
	std::string image_path;
	std::string pressed_image_path;
};

class KeyConfigButton
{
public:
	std::string image_path;
	std::string pressed_image_path;
	sf::Keyboard::Key* key;
	bool is_pressed = false;
};