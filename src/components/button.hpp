#pragma once
#include "SFML/Window/Keyboard.hpp"
#include "ecs/entity.hpp"
#include <functional>
#include <optional>
#include <string>

class ShortcutKey
{
public:
	sf::Keyboard::Key key;
	std::optional<sf::Keyboard::Key> required_modifier_key = std::nullopt;
};

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

class OnPressed
{
public:
	std::function<void(void)> func;
};
class CanDisableButton
{
public:
	std::function<bool(void)> func;
	sf::Uint8 deactivation_alpha = 50;
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
	int* slider_value; //float normalized from 0-100.
	Entity slider_text;
	Entity slider_button;
	Entity slider_bar;
};