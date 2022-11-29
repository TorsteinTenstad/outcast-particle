#pragma once
#include "PCH.hpp"

#define PI 3.141593

static float FakeSigmoid(float x)
{
	assert(x >= 0);
	assert(x <= 1);
	if (x < 0.5)
	{
		return pow(2 * x, 3) / 2;
	}
	else
	{
		return 1 - abs(pow(2 * x - 2, 3)) / 2;
	}
}

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

template <class T>
static int Sign(T x)
{
	if (x < 0)
	{
		return -1;
	}
	return 1;
}

// NOTE: The array is assumed sorted
template <class T, std::size_t SIZE>
static unsigned FindClosest(const std::array<T, SIZE>& sorted_arr, const T& value)
{
	for (unsigned i = 0; i < sorted_arr.size(); ++i)
	{
		if (value < sorted_arr[i])
		{
			if (i == 0)
			{
				return i;
			}
			if (value - sorted_arr[i - 1] < sorted_arr[i] - value)
			{
				return i - 1;
			}
			else
			{
				return i;
			}
		}
	}
	return sorted_arr.size() - 1;
}