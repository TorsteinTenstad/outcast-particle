#pragma once
#include "SFML/Graphics/Color.hpp"
#include "grid_entities_manager.hpp"
#include <sstream>
#include <string>
#include <vector>

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

template <std::size_t N>
inline std::string ToString(const std::array<uint8_t, N>& arr)
{
	return std::string(arr.begin(), arr.end());
}

template <std::size_t N>
inline void FromString(std::array<uint8_t, N>, std::string& str)
{
	std::array<uint8_t, N> arr;
	std::copy(str.begin(), str.end(), arr.begin());
}

inline std::string ToString(const GridEntitiesData& x)
{
	x.data_texture_.copyToImage().saveToFile("level_data\\" + x.savefile_identifier_ + ".png");
	return "level_data\\\\" + x.savefile_identifier_ + ".png";
}

inline void FromString(GridEntitiesData& x, std::string& str)
{
	sf::Image image;
	image.loadFromFile(str);
	for (size_t i = 0; i < x.WIDTH; i++)
	{
		for (size_t j = 0; j < x.HEIGHT; j++)
		{
			sf::Color color = image.getPixel(i, j);
			x.SetValue(i, j, 0, color.r);
			x.SetValue(i, j, 1, color.g);
			x.SetValue(i, j, 2, color.b);
			x.SetValue(i, j, 3, color.a);
		}
	}
}

inline std::string ToString(std::string x)
{
	return x;
}
inline std::string ToString(int x)
{
	return std::to_string(x);
}
inline std::string ToString(float x)
{
	std::stringstream s;
	s << x;
	return s.str();
}
inline std::string ToString(bool x)
{
	return std::to_string(x);
}
inline std::string ToString(sf::Vector2f x)
{
	return "(" + ToString(x.x) + "," + ToString(x.y) + ")";
}
inline std::string ToString(sf::Color x)
{
	return "(" + ToString(x.r) + "," + ToString(x.g) + "," + ToString(x.b) + ")";
}

inline void FromString(std::string& x, std::string s)
{
	x = s;
}
inline void FromString(int& x, std::string s)
{
	x = stod(s);
}
inline void FromString(sf::Keyboard::Key& x, std::string s)
{
	FromString((int&)x, s);
}
inline void FromString(float& x, std::string s)
{
	x = stod(s);
}
inline void FromString(bool& x, std::string s)
{
	x = (s == "1");
}
inline void FromString(sf::Vector2f& x, std::string s)
{
	std::vector<std::string> x_y = SplitString(s.substr(1, s.length() - 1), ",");
	FromString(x.x, x_y[0]);
	FromString(x.y, x_y[1]);
}
inline void FromString(sf::Color& x, std::string s)
{
	x = sf::Color(255, 0, 255);
}