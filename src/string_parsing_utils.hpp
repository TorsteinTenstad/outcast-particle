#pragma once
#include "PCH.hpp"

std::string GetSubstrBetween(std::string s, std::string prefix, std::string postfix)
{
	return "";
}

std::vector<std::string> SplitString(std::string s, std::string delimiter)
{
	int last_delimiter_idx = 0;
	std::vector<std::string> substrings;
	int i = 0;
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

template <typename T>
std::string ToString(T x)
{
	return std::to_string(x);
}
template std::string ToString(int);
template std::string ToString(float);
template std::string ToString(bool);

template <>
std::string ToString(sf::Vector2f x)
{
	return "(" + ToString(x.x) + "," + ToString(x.y) + ")";
}

template <typename T>
void FromString(T& x, std::string s)
{
	x = (T)stod(s);
}
template void FromString(int&, std::string);
template void FromString(float&, std::string);
template void FromString(bool&, std::string);

template <>
void FromString(sf::Vector2f& x, std::string s)
{
}