#pragma once
#include "SFML/Window/Keyboard.hpp"
#include <functional>
#include <string>

class OnReleasedThisFrame
{
public:
	std::function<void(void)> func;
};

class OnHoveredStartedThisFrame
{
public:
	std::function<void(void)> func;
};

class OnHovered
{
public:
	std::function<void(void)> func;
};

class KeyConfigButton
{
public:
	std::string image_path;
	std::string pressed_image_path;
	sf::Keyboard::Key* key;
	bool is_pressed = false;
};