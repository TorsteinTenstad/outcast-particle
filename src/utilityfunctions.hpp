#pragma once
#include "PCH.hpp"

#define PI 3.141593

static float Magnitude(sf::Vector2f v)
{
	return std::sqrt((v.x * v.x) + (v.y * v.y));
}

static sf::Vector2f Normalized(sf::Vector2f v)
{
	return v / Magnitude(v);
}

static sf::Vector2f Abs(sf::Vector2f v)
{
	return sf::Vector2f(abs(v.x), abs(v.y));
}

static sf::Vector2f GetQuarterTurnRotation(sf::Vector2f v)
{
	return sf::Vector2f(-v.y, v.x);
}

static float Dot(sf::Vector2f u, sf::Vector2f v)
{
	return u.x * v.x + u.y * v.y;
}

template <typename T>
static int Sign(T x)
{
	if (x > 0)
	{
		return 1;
	}
	if (x < 0)
	{
		return -1;
	}
	return 0;
}