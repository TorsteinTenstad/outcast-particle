#pragma once
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <sstream>
#include <string>
#include <vector>

std::string GetSubstrBetween(std::string s, std::string prefix, std::string postfix);
std::vector<std::string> SplitString(std::string s, std::string delimiter);

std::string ToString(std::string x);
std::string ToString(float x);
std::string ToString(sf::Color x);

void FromString(std::string& x, std::string s);
void FromString(sf::Keyboard::Key& x, std::string s);
void FromString(bool& x, std::string s);
void FromString(sf::Color& x, std::string s);

template <class T>
void FromString(T& x, std::string s)
{
	x = stod(s);
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
void FromString(sf::Vector2<T>& x, std::string s)
{
	std::vector<std::string> x_y = SplitString(s.substr(1, s.length() - 1), ",");
	FromString(x.x, x_y[0]);
	FromString(x.y, x_y[1]);
}