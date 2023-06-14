#pragma once
#include "utils/string_parsing.hpp"
#include <cassert>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

std::optional<std::string> GetSubstrBetween(std::string s, std::optional<std::string> prefix, std::optional<std::string> postfix)
{
	auto prefix_idx = prefix.has_value() ? s.find(prefix.value()) : 0;
	auto prefix_len = prefix.has_value() ? prefix.value().length() : 0;
	if (prefix_idx == std::string::npos) { return std::nullopt; }
	s = s.substr(prefix_idx + prefix_len);
	if (!postfix.has_value()) { return s; }
	auto postfix_idx = s.find(postfix.value());
	if (postfix_idx == std::string::npos) { return std::nullopt; }
	s = s.substr(0, postfix_idx);
	return s;
}
std::vector<std::string> SplitString(std::string s, std::string delimiter)
{
	int last_delimiter_idx = 0;
	std::vector<std::string> substrings;
	unsigned i = 0;
	while (i < s.size())
	{
		if (s[i] == delimiter[0])
		{
			substrings.push_back(s.substr(last_delimiter_idx, i - last_delimiter_idx));
			last_delimiter_idx = i + 1;
			i++;
		}
		i++;
	}
	substrings.push_back(s.substr(last_delimiter_idx, s.size()));
	return substrings;
}
bool IsNumeric(const std::string& s)
{
	return !s.empty() && s.find_first_not_of("0123456789") == std::string::npos;
}

std::string ToString(std::string x)
{
	return x;
}
std::string ToString(float x)
{
	std::stringstream s;
	s << x;
	return s.str();
}
std::string ToString(sf::Color x)
{
	return "(" + ToString(x.r) + "," + ToString(x.g) + "," + ToString(x.b) + ")";
}
Error_t FromString(std::string& x, std::string s)
{
	x = s;
	return SUCCESS;
}
Error_t FromString(sf::Keyboard::Key& x, std::string s)
{
	FromString((int&)x, s);
	return SUCCESS;
}
Error_t FromString(bool& x, std::string s)
{
	x = (s == "1");
	return SUCCESS;
}
Error_t FromString(sf::Color& x, std::string s)
{
	x = sf::Color(255, 0, 255);
	assert(false);
	return SUCCESS;
}

std::string UTF32ToUTF8(sf::Uint32 utf32_char)
{
	std::vector<char> utf8_chars;
	if (utf32_char <= 0x7F)
	{
		utf8_chars.push_back(static_cast<char>(utf32_char));
	}
	else if (utf32_char <= 0x7FF)
	{
		utf8_chars.push_back(static_cast<char>(0xC0 | (utf32_char >> 6)));
		utf8_chars.push_back(static_cast<char>(0x80 | (utf32_char & 0x3F)));
	}
	else if (utf32_char <= 0xFFFF)
	{
		utf8_chars.push_back(static_cast<char>(0xE0 | (utf32_char >> 12)));
		utf8_chars.push_back(static_cast<char>(0x80 | ((utf32_char >> 6) & 0x3F)));
		utf8_chars.push_back(static_cast<char>(0x80 | (utf32_char & 0x3F)));
	}
	else if (utf32_char <= 0x10FFFF)
	{
		utf8_chars.push_back(static_cast<char>(0xF0 | (utf32_char >> 18)));
		utf8_chars.push_back(static_cast<char>(0x80 | ((utf32_char >> 12) & 0x3F)));
		utf8_chars.push_back(static_cast<char>(0x80 | ((utf32_char >> 6) & 0x3F)));
		utf8_chars.push_back(static_cast<char>(0x80 | (utf32_char & 0x3F)));
	}
	else
	{ // Invalid Unicode code point
		assert(false);
	}
	return std::string(utf8_chars.begin(), utf8_chars.end());
}