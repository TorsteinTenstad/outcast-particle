#pragma once
#include <cstddef>
#include <fstream>

#define PI 3.141593

static float Magnitude(sf::Vector2f v)
{
	return std::sqrt((v.x * v.x) + (v.y * v.y));
}

static float Angle(sf::Vector2f v)
{
	return std::atan2(v.y, v.x);
}

//Rounds x to the nearest integer multiple of y
static float RoundToNearest(float x, float y)
{
	float rest = std::fmod(x, y);
	if (rest < y / 2)
	{
		return x - rest;
	}
	else
	{
		return x + y - rest;
	}
}

template <class T>
static void SaveToBinaryFile(const T& obj, const char* path)
{
	auto* bytes = reinterpret_cast<const char*>(&obj);
	std::ofstream file;
	file.open(path);
	file.write(bytes, sizeof(T));
}

template <class T>
static void LoadFromBinaryFile(T& obj, const char* path)
{
	auto* bytes = reinterpret_cast<char*>(&obj);
	std::ifstream file;
	file.open(path);
	file.read(bytes, sizeof(T));
}
