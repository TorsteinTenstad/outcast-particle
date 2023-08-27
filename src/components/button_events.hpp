#pragma once
#include "SFML/Window/Keyboard.hpp"
#include <string>

class ReceivesButtonEvents
{
public:
	bool block_lower_priority_entities = true;
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