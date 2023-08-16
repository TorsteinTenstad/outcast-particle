#pragma once
#include "SFML/Window/Keyboard.hpp"
#include <string>

std::string GetDateTimeIdentifier();
void RemoveChars(std::string& s, const std::string& chars);
std::string HumanName(sf::Keyboard::Key key);
std::string RightPad(const std::string& original, size_t total_length, char pad_char = ' ');
std::string LeftPad(const std::string& original, size_t total_length, char pad_char = ' ');
std::string BoolToStringAsEnabledOrDisabled(bool is);
std::string CreateBadgeText(float f, int precision);