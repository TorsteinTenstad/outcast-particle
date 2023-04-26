#pragma once
#include "SFML/Window/Keyboard.hpp"
#include <string>

void RemoveChars(std::string& s, const std::string& chars);
std::string HumanName(sf::Keyboard::Key key);
std::string LeftShiftString(std::string string, int max_size);
std::string RightShiftString(std::string string, int max_size);
std::string BoolToStringAsEnabledOrDisabled(bool is);
std::string CreateBadgeText(float f, int precision);