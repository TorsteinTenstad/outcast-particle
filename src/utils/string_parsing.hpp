#pragma once
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "error.hpp"
#include <cassert>
#include <fstream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

std::optional<std::string> GetSubstrBetween(std::string s, std::optional<std::string> prefix, std::optional<std::string> postfix);
std::vector<std::string> SplitString(std::string s, std::string delimiter);
bool IsNumeric(const std::string& s);

std::string ToString(std::string x);
std::string ToString(float x);
std::string ToString(sf::Color x);

Error_t FromString(std::string& x, std::string s);
Error_t FromString(sf::Keyboard::Key& x, std::string s);
Error_t FromString(bool& x, std::string s);
Error_t FromString(sf::Color& x, std::string s);

std::string UTF32ToUTF8(sf::Uint32 utf32_char);

template <class T>
Error_t FromString(T& x, const std::string& s)
{
	try
	{
		x = std::stod(s);
		return SUCCESS;
	}
	catch (const std::invalid_argument&)
	{
		return ERROR;
	}
	catch (const std::out_of_range&)
	{
		return ERROR;
	}
}

template <class T>
std::string ToString(T x)
{
	return std::to_string(x);
}
template <class T>
std::string ToString(sf::Vector2<T> x)
{
	return "(" + ToString(x.x) + "," + ToString(x.y) + ")";
}
template <class T>
Error_t FromString(sf::Vector2<T>& x, std::string s)
{
	std::vector<std::string> x_y = SplitString(s.substr(1, s.length() - 1), ",");
	Error_t err;
	err += FromString(x.x, x_y[0]);
	err += FromString(x.y, x_y[1]);
	return err;
}

template <class... T>
std::string ToString(const std::tuple<T...>& tuple)
{
	if constexpr (sizeof...(T) == 0)
	{
		assert(false);
		return "()";
	}
	std::string result = "(";
	((result += (ToString(std::get<T>(tuple)) + ",")), ...);
	result.resize(result.size() - 1); // Remove the trailing comma and space
	result += ")";
	return result;
}

template <class... T>
Error_t FromString(std::tuple<T...>& tuple, std::string s)
{
	std::vector<std::string> elements;

	// Remove parentheses from the string
	s.erase(std::remove(s.begin(), s.end(), '('), s.end());
	s.erase(std::remove(s.begin(), s.end(), ')'), s.end());

	// Splitting the string by ","
	std::istringstream iss(s);
	std::string token;
	while (std::getline(iss, token, ','))
	{
		elements.push_back(token);
	}

	// Check if the number of elements matches the tuple size
	if (elements.size() != sizeof...(T))
	{
		return ERROR;
	}

	std::size_t index = 0;
	Error_t err = ((FromString(std::get<T>(tuple), elements[index++])) + ...);
	return err;
}

template <class K, class V>
std::string ToString(const std::map<K, V>& map)
{
	std::string str_rep;
	for (const auto& i : map)
	{
		str_rep += ToString(i.first);
		str_rep += ";";
		str_rep += ToString(i.second);
		str_rep += "\n";
	}
	return str_rep;
}

template <class K, class V>
Error_t FromString(std::map<K, V>& map, std::string s)
{
	std::vector<std::string> keys_and_values = SplitString(s, "\n");
	K key;
	V value;
	Error_t err;
	for (auto& i : keys_and_values)
	{
		std::vector<std::string> pair = SplitString(i, ";");
		if (pair.size() < 2)
		{
			err += ERROR;
			continue;
		}
		if (pair.size() > 2)
		{
			err += ERROR;
		}
		err += FromString(key, pair[0]);
		err += FromString(value, pair[1]);
		map[key] = value;
	}
	return err;
}

template <class T>
void ToFile(const T& x, const std::filesystem::path& file)
{
	std::ofstream f(file);
	if (f.fail())
	{
		assert(false);
		return;
	}
	f << ToString(x);
}

template <class T>
Error_t FromFile(T& x, const std::filesystem::path& file)
{
	std::ifstream f(file);

	if (f.fail()) { return ERROR; }

	std::stringstream buffer;
	buffer << f.rdbuf();
	Error_t err = FromString(x, buffer.str());
	return err;
}
