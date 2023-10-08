#pragma once
#include "SFML/Window/Keyboard.hpp"
#include "ecs/entity.hpp"
#include <array>
#include <functional>
#include <optional>
#include <string>

class ShortcutKey
{
public:
	sf::Keyboard::Key key;
	bool requires_ctrl_modifier = false;
};

class OnReleasedThisFrame
{
public:
	std::function<void(void)> func;
};

class OnPressedThisFrame
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

class OnPressed
{
public:
	std::function<void(void)> func;
};
class CanDisableButton
{
public:
	std::function<bool(void)> func;
	sf::Uint8 deactivation_alpha = 25;
	bool regain_button_events = true;
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

class SliderButton
{
public:
	int* slider_value; //int within range
	std::array<int, 2> range = { 0, 100 };
	Entity slider_text;
	Entity slider_button;
	Entity slider_bar;
};