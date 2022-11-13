#include "level.hpp"

class BPEntity
{
	ClickedOn clicked_on = {};
	// [Serialize]
	Position position = { sf::Vector2f(0, 0) };
};

class BPStaticParticle : public BPEntity
{
	ChargeDependentDrawInfo charge_dependent_draw_info = { "content\\particle_100_red+.png", "content\\particle_100_green.png", "content\\particle_100_green-.png" };
	Editable editable = { true, false, false, sf::Vector2f(0, 0), false };
	Radius radius = { 50 };

	// [Serialize]
	Charge charge = { 0 };
};

class BPMovingParticle : public BPStaticParticle
{
	Acceleration acceleration = {};
	ReceivedForces received_forces = {};
	Editable editable = { true, true, false, sf::Vector2f(0, 0), false };
	Intersection intersection = {};
	Collision collision = {};

	// [Serialize]
	Velocity velocity = { sf::Vector2f(0, 0) };
};

class BPPlayer : public BPMovingParticle
{
	ChargeDependentDrawInfo charge_dependent_draw_info = { "content\\particle_100_blue+.png", "content\\particle_100_blue.png", "content\\particle_100_blue-.png" };
	Player player = {};
};

class BPWall : public BPEntity
{
	DrawInfo draw_info = { "content\\block.png" };
	Collision collision = { 0.2 };
	Editable editable = { false, false, true, sf::Vector2f(0, 0), false };

	// [Serialize]
	WidthAndHeight width_and_height = {};
};

class BPGoal : public BPEntity
{
	DrawInfo draw_info = { "content\\goal.png" };
	Goal goal = {};
	Editable editable = { false, false, true, sf::Vector2f(0, 0), false };
	KillOnIntersection kill_on_intersection = {};

	// [Serialize]
	WidthAndHeight width_and_height = {};
};

class BPLaser : public BPEntity
{
	OrientationDependentDrawInfo orientation_dependent_drawinfo = { "content\\laser_horisontal.png", "content\\laser_vertical.png" };
	Editable editable = { false, false, true, sf::Vector2f(0, 0), false };
	KillOnIntersection kill_on_intersection = {};

	// [Serialize]
	WidthAndHeight width_and_height = {};
};

class BPElectricField : public BPEntity
{
	DrawInfo draw_info = { "content\\electric_field.png" };
	Editable editable = { false, false, true, sf::Vector2f(0, 0), false };

	// [Serialize]
	ElectricField electric_field = {};
	// [Serialize]
	WidthAndHeight width_and_height = {};
};