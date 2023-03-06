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
	sf::Keyboard::Key* key;
	std::string* button_text;
};

class BinaryOptionsButton
{
public:
	std::string* button_text;
};