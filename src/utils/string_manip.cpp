#include "utils/string_manip.hpp"
#include <cassert>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

std::string GetDateTimeIdentifier()
{
	auto now = std::chrono::system_clock::now();
	std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
	std::tm* localTime = std::localtime(&nowTime);

	std::ostringstream oss;
	oss << std::put_time(localTime, "%Y%m%d_%H%M%S");

	return oss.str();
}

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

std::string LeftPad(const std::string& original, size_t total_length, char pad_char)
{
	if (total_length <= original.size())
	{
		assert(false);
		return original;
	}

	std::string padding(total_length - original.size(), pad_char);
	return padding + original;
}

std::string RightPad(const std::string& original, size_t total_length, char pad_char)
{
	if (total_length <= original.size())
	{
		assert(false);
		return original;
	}

	std::string padding(total_length - original.size(), pad_char);
	return original + padding;
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
	//ss << "s";
	return ss.str();
}