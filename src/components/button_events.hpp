#pragma once
#include "SFML/Window/Keyboard.hpp"
#include <string>

class ReceivesButtonEvents
{
};

class ShortcutKey
{
public:
	sf::Keyboard::Key key;
};

class Pressed
{
};

class Hovered
{
};

class HoveredStartedThisFrame
{
};

class PressedThisFrame
{
};

class ReleasedThisFrame
{
};