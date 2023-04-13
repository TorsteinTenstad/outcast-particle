#pragma once
#include "SFML/System/Vector2.hpp"
#include <array>

#define PI 3.141593f

float Smoothstep(float a, float b, float x);
float Smoothstep(float x);
float Ease(float x, float undershoot, float overshoot);

std::array<int, 2> Vector2iToArray(sf::Vector2i v);

float Magnitude(sf::Vector2f v);
float Angle(sf::Vector2f v);
sf::Vector2f Vector2fFromPolar(float magnitude, float radians);
sf::Vector2f Normalized(sf::Vector2f v);
sf::Vector2f Abs(sf::Vector2f v);
sf::Vector2f GetRotated(sf::Vector2f v, float radians);
sf::Vector2f GetQuarterTurnRotation(sf::Vector2f v);
float Dot(sf::Vector2f u, sf::Vector2f v);

template <class T>
int Sign(T x)
{
	if (x < 0)
	{
		return -1;
	}
	return 1;
}

template <class T>
T Clamp(T val, T min, T max)
{
	return std::min(max, std::max(min, val));
}