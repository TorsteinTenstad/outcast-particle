#include "utils/string_manip.hpp"
#include <cassert>
#include <iomanip>
#include <sstream>

void RemoveChars(std::string& s, const std::string& chars)
{
	size_t pos = 0;
	while ((pos = s.find_first_of(chars, pos)) != std::string::npos)
	{
		s.erase(pos, 1);
	}
}

std::string HumanName(sf::Keyboard::Key key)
{
	if (sf::Keyboard::A <= key && key <= sf::Keyboard::Z)
	{
		return std::string(1, 'A' + key);
	}
	if (sf::Keyboard::Num0 <= key && key <= sf::Keyboard::Num9)
	{
		return std::string(1, '0' + key);
	}
	switch (key)
	{
		case sf::Keyboard::Escape:
			return "Escape";
		case sf::Keyboard::LControl:
			return "LControl";
		case sf::Keyboard::LShift:
			return "LShift";
		case sf::Keyboard::LAlt:
			return "LAlt";
		case sf::Keyboard::RControl:
			return "RControl";
		case sf::Keyboard::RShift:
			return "RShift";
		case sf::Keyboard::RAlt:
			return "RAlt";
		case sf::Keyboard::Space:
			return "Space";
		case sf::Keyboard::Enter:
			return "Enter";
		case sf::Keyboard::Backspace:
			return "Backspace";
		case sf::Keyboard::Tab:
			return "Tab";
		case sf::Keyboard::Delete:
			return "Delete";
		case sf::Keyboard::Comma:
			return ",";
		case sf::Keyboard::Period:
			return "Period";

		default:
			return "Unknown key";
	}
}

std::string LeftShiftString(std::string string, int max_size)
{
	assert(string.size() <= max_size);
	string.append(max_size - string.size(), ' ');
	return (string);
}

std::string RightShiftString(std::string string, int max_size)
{
	assert(string.size() <= max_size);
	size_t size_difference = size_t(max_size) - string.size();
	std::string shifted_string = std::string(size_difference, ' ');
	shifted_string += string;

	return shifted_string;
}

std::string BoolToStringAsEnabledOrDisabled(bool x)
{
	if (x)
	{
		return "Enabled";
	}
	else
	{
		return "Disabled";
	}
}

std::string CreateBadgeText(float f, int precision)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(precision);
	ss << f;
	ss << "s";
	return ss.str();
}