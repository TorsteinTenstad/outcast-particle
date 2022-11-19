#include "level.hpp"

class BPEntity
{
	ClickedOn clicked_on = {};
	// [Serialize]
	Position position = { sf::Vector2f(0, 0) };
};

class BPTileBasedEntity : public BPEntity
{
	Editable editable = { false, false, 120, sf::Vector2f(0, 0), false };

	// [Serialize]
	WidthAndHeight width_and_height = {};
};

class BPStaticParticle : public BPEntity
{
	ChargeDependentDrawInfo charge_dependent_draw_info = { "content\\particle_red+.png", "content\\particle_red.png", "content\\particle_green-.png" };
	Editable editable = { true, false, 0, sf::Vector2f(0, 0), false };
	Radius radius = { 120 };

	// [Serialize]
	Charge charge = { 0 };
};

class BPMovingParticle : public BPStaticParticle
{
	Editable editable = { true, true, 0, sf::Vector2f(0, 0), false };
	Acceleration acceleration = {};
	ReceivedForces received_forces = {};
	Intersection intersection = {};
	Collision collision = {};

	// [Serialize]
	Velocity velocity = { sf::Vector2f(0, 0) };
};

class BPPlayer : public BPMovingParticle
{
	ChargeDependentDrawInfo charge_dependent_draw_info = { "content\\particle_blue+.png", "content\\particle_blue.png", "content\\particle_blue-.png" };
	Player player = {};
};

class BPLaser : public BPEntity
{
	OrientationDependentDrawInfo orientation_dependent_draw_info = { "content\\laser_horisontal.png", "content\\laser_vertical.png" };
	Editable editable = { false, false, 60, sf::Vector2f(0, 0), false };
	KillOnIntersection kill_on_intersection = {};

	// [Serialize]
	WidthAndHeight width_and_height = {};
};

class BPWall : public BPTileBasedEntity
{
	DrawInfo draw_info = { "content\\block.png", false, 1, 0 };
	Collision collision = { 0.2 };
};

class BPGoal : public BPTileBasedEntity
{
	DrawInfo draw_info = { "content\\goal.png", false, -2, 0 };
	Goal goal = {};
	KillOnIntersection kill_on_intersection = {};
};

class BPElectricField : public BPTileBasedEntity
{
	DrawInfo draw_info = { "content\\electric_field.png", false, -5, 0 };

	// [Serialize]
	ElectricField electric_field = {};
};

class BPMagneticField : public BPTileBasedEntity
{
	DrawInfo draw_info = { "content\\magnetic_field.png", false, -5, 0 };

	// [Serialize]
	MagneticField magnetic_field = {};
};
