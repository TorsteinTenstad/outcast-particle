#pragma once
#include "string_parsing_utils.hpp"
#include <sstream>
#include <string>
#include <vector>

std::string GetSubstrBetween(std::string s, std::string prefix, std::string postfix)
{
	auto prefix_idx = s.find(prefix);
	s = s.substr(prefix_idx + prefix.length());
	auto postfix_idx = s.find(postfix);
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
void FromString(std::string& x, std::string s)
{
	x = s;
}
void FromString(sf::Keyboard::Key& x, std::string s)
{
	FromString((int&)x, s);
}
void FromString(bool& x, std::string s)
{
	x = (s == "1");
}
void FromString(sf::Color& x, std::string s)
{
	x = sf::Color(255, 0, 255);
}