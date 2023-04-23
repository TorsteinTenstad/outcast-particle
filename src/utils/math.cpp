#include "utils/math.hpp"
#include "SFML/System/Vector2.hpp"
#include "math.hpp"
#include <cassert>
#include <math.h>

float& GetX(sf::Vector2f& v)
{
	return v.x;
}

float& GetY(sf::Vector2f& v)
{
	return v.y;
}

float Smoothstep(float a, float b, float x)
{
	float t = Clamp((x - a) / (b - a), 0.f, 1.f);
	return t * t * (3.0 - 2.0 * t);
}

float Smoothstep(float x)
{
	return Smoothstep(0, 1, x);
}

float Ease(float x, float undershoot, float overshoot)
{
	x = Clamp(x, 0.f, 1.f);
	float a = overshoot * x * x;
	float b = 1 - undershoot * (x - 1) * (x - 1);
	float s = Smoothstep(0, 1, x);
	float result = a * (1 - s) + b * s;
	assert(0 <= result);
	return result;
}

std::array<int, 2> Vector2iToArray(sf::Vector2i v)
{
	return { v.x, v.y };
};

float Magnitude(sf::Vector2f v)
{
	return sqrt((v.x * v.x) + (v.y * v.y));
}

float Angle(sf::Vector2f v)
{
	return atan2(v.y, v.x);
}
sf::Vector2f Vector2fFromPolar(float magnitude, float radians)
{
	return magnitude * sf::Vector2f(cos(radians), sin(radians));
}

sf::Vector2f Normalized(sf::Vector2f v)
{
	return v / Magnitude(v);
}

sf::Vector2f Abs(sf::Vector2f v)
{
	return sf::Vector2f(abs(v.x), abs(v.y));
}

sf::Vector2f GetRotated(sf::Vector2f v, float radians)
{
	float new_x = cos(radians) * v.x - sin(radians) * v.y;
	float new_y = sin(radians) * v.x + cos(radians) * v.y;
	return sf::Vector2f(new_x, new_y);
}

sf::Vector2f GetQuarterTurnRotation(sf::Vector2f v)
{
	return sf::Vector2f(-v.y, v.x);
}

float Dot(sf::Vector2f u, sf::Vector2f v)
{
	return u.x * v.x + u.y * v.y;
}