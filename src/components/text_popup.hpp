#pragma once

#include "SFML/Window/Keyboard.hpp"
#include <string>

class TextPopup
{
};
class TextPopupSpawner
{
public:
	std::string content;
	float alert_timer = 0;
	sf::Keyboard::Key key = sf::Keyboard::Key::Unknown;
};