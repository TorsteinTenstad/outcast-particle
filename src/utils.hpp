#pragma once
#include "PCH.hpp"

#define PI 3.141593

float FakeSigmoid(float x);

float Magnitude(sf::Vector2f v);

sf::Vector2f Normalized(sf::Vector2f v);

sf::Vector2f Abs(sf::Vector2f v);

sf::Vector2f GetQuarterTurnRotation(sf::Vector2f v);

float Dot(sf::Vector2f u, sf::Vector2f v);

std::vector<sf::Vector2f> GridHelper(int n_elements, int n_columns, float eliment_w, float eliment_h, float spacing);

template <class T>
int Sign(T x)
{
	if (x < 0)
	{
		return -1;
	}
	return 1;
}

// NOTE: The array is assumed sorted
template <class T, std::size_t SIZE>
unsigned FindClosest(const std::array<T, SIZE>& sorted_arr, const T& value)
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
