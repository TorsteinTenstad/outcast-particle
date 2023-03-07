#pragma once
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <array>
#include <iterator>
#include <map>
#include <string>
#include <vector>

#define PI 3.141593

float Smoothstep(float a, float b, float x);
float Ease(float x, float undershoot, float overshoot);

std::string GetGroupNameFromId(const std::string& level_id);
std::string GetGroupDisplayNameFromGroupName(const std::string& group_name);
std::string GetGroupDisplayNameFromId(const std::string& level_id);
std::string GetLevelDisplayNameFromId(const std::string& level_id);

bool IsMenu(const std::string& level_id);

float FakeSigmoid(float x);

std::array<int, 2> Vector2iToArray(sf::Vector2i v);

float Magnitude(sf::Vector2f v);
float Angle(sf::Vector2f v);
sf::Vector2f Normalized(sf::Vector2f v);
sf::Vector2f Abs(sf::Vector2f v);
sf::Vector2f GetQuarterTurnRotation(sf::Vector2f v);
float Dot(sf::Vector2f u, sf::Vector2f v);

std::vector<sf::Vector2f> GridHelper(int n_elements, int n_columns, float spacing_w, float spacing_h);

std::string HumanName(sf::Keyboard::Key key);

std::vector<std::string> LeftOrRightShiftString(std::vector<std::string> strings, int max_size, bool right_shift);
std::string BoolToStringAsEnabledOrDisabled(bool is);

template <class K, class V>
typename std::map<K, V>::const_iterator NextInMap(const std::map<K, V>& m, K key)
{
	assert(m.count(key) == 1);
	auto it = m.find(key);
	++it;
	if (it == m.end())
	{
		return --it;
	}
	return it;
}
template <class K, class V>
typename std::map<K, V>::const_iterator PrevInMap(const std::map<K, V>& m, K key)
{
	assert(m.count(key) == 1);
	auto it = m.find(key);
	if (it == m.begin())
	{
		return it;
	}
	--it;
	return it;
}

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
int Clamp(T val, T min, T max)
{
	if (val < min)
	{
		return min;
	}
	if (val > max)
	{
		return max;
	}
	return val;
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
