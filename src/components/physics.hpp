#pragma once
#include "PCH.hpp"

class Position
{
public:
	sf::Vector2f position = sf::Vector2f(0, 0);

	std::string to_string()
	{
		return "(" + std::to_string(position.x) + "," + std::to_string(position.x) + ")";
	}
};

class Velocity
{
public:
	sf::Vector2f velocity = sf::Vector2f(0, 0);
};

class Acceleration
{
public:
	sf::Vector2f acceleration = sf::Vector2f(0, 0);
};

class ReceivedForces
{
public:
	float mass = 1;
	sf::Vector2f electric_force = sf::Vector2f(0, 0);
	sf::Vector2f player_force = sf::Vector2f(0, 0);
};

class Charge
{
public:
	float charge = 0;
};
