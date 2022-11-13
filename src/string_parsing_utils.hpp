#pragma once
#include "PCH.hpp"

inline std::string GetSubstrBetween(std::string s, std::string prefix, std::string postfix)
{
	auto prefix_idx = s.find(prefix);
	s = s.substr(prefix_idx + prefix.length());
	auto postfix_idx = s.find(postfix);
	s = s.substr(0, postfix_idx);
	return s;
}

inline std::vector<std::string> SplitString(std::string s, std::string delimiter)
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

inline std::string ToString(int x)
{
	return std::to_string(x);
}
inline std::string ToString(float x)
{
	return std::to_string(x);
}
inline std::string ToString(bool x)
{
	return std::to_string(x);
}
inline std::string ToString(sf::Vector2f x)
{
	return "(" + ToString(x.x) + "," + ToString(x.y) + ")";
}
inline std::string ToString(std::string x)
{
	return x;
}

inline void FromString(std::string& x, std::string s)
{
	x = s;
}
inline void FromString(float& x, std::string s)
{
	x = stod(s);
}
inline void FromString(bool& x, std::string s)
{
	x = (s == "true");
}
inline void FromString(sf::Vector2f& x, std::string s)
{
	std::vector<std::string> x_y = SplitString(s.substr(1, s.length() - 1), ",");
	FromString(x.x, x_y[0]);
	FromString(x.y, x_y[1]);
}