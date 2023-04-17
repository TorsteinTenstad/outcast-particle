#pragma once
#include "SFML/System/Vector2.hpp"
#include "position.hpp"
#include <optional>

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

class Mass
{
public:
	float mass = 1;
};

class ReceivedForces
{
public:
	sf::Vector2f electric_force = sf::Vector2f(0, 0);
	sf::Vector2f player_force = sf::Vector2f(0, 0);
	sf::Vector2f electric_field_force = sf::Vector2f(0, 0);
	sf::Vector2f magnetic_field_force = sf::Vector2f(0, 0);
	sf::Vector2f gravitational_force = sf::Vector2f(0, 0);
};

class Charge
{
public:
	float charge = 0;
};

class ElectricField
{
public:
	sf::Vector2f field_vector = sf::Vector2f(0, 0);
};

class MagneticField
{
public:
	float field_strength = 5;
};