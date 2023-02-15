#pragma once
#include "SFML/Window/Keyboard.hpp"
#include <string>

class ReceivesMouseEvents
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
class PressedImagePath
{
public:
	std::string image_path;
	std::string pressed_image_path;
};
class PressedThisFrame
{
};
class ReleasedThisFrame
{
};