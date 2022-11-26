#include "level.hpp"

class BPEntity
{
	ClickedOn clicked_on = {};
	// [Serialize]
	Position position = { sf::Vector2f(0, 0) };
};

class BPTileBasedEntity : public BPEntity
{
	Editable editable = { true, false, false, 120, sf::Vector2f(0, 0), false };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(120, 120) };
};

class BPStaticParticle : public BPEntity
{
	ChargeDependentDrawInfo charge_dependent_draw_info = { "content\\textures\\particle_red+.png", "content\\textures\\particle_blue.png", "content\\textures\\particle_green-.png" };
	Editable editable = { true, true, false, 0, sf::Vector2f(0, 0), false };
	Radius radius = { 120 };

	// [Serialize]
	Charge charge = { 100000 };
};

class BPMovingParticle : public BPStaticParticle
{
	Editable editable = { true, true, true, 0, sf::Vector2f(0, 0), false };
	Acceleration acceleration = {};
	ReceivedForces received_forces = {};
	Intersection intersection = {};
	Collision collision = {};
	Trail trail = {};

	// [Serialize]
	Velocity velocity = { sf::Vector2f(0, 0) };
};

class BPPlayer : public BPMovingParticle
{
	ChargeDependentDrawInfo charge_dependent_draw_info = { "content\\textures\\particle_blue+.png", "content\\textures\\particle_blue.png", "content\\textures\\particle_blue-.png" };

	// [Serialize]
	Player player = { true, true, 500 };
};

class BPLaser : public BPEntity
{
	OrientationDependentDrawInfo orientation_dependent_draw_info = { "content\\textures\\laser_horisontal.png", "content\\textures\\laser_vertical.png" };
	Editable editable = { true, false, false, 60, sf::Vector2f(0, 0), false };
	KillOnIntersection kill_on_intersection = {};

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(120, 60) };
};

class BPWall : public BPTileBasedEntity
{
	DrawInfo draw_info = { "content\\textures\\block.png", false, 1, 0 };
	SoundInfo sound_info = { "content\\sounds\\thud.wav" };
	Collision collision = { 0.2, 75 };
};

class BPGoal : public BPTileBasedEntity
{
	DrawInfo draw_info = { "content\\textures\\goal.png", false, -2, 0 };
	Goal goal = {};
	KillOnIntersection kill_on_intersection = {};
	SoundInfo sound_info = { "content\\sounds\\fanfare.wav" };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(240, 240) };
};

class BPElectricField : public BPTileBasedEntity
{
	DrawInfo draw_info = { "content\\textures\\electric_field.png", false, -5, 0 };

	// [Serialize]
	ElectricField electric_field = { sf::Vector2f(0, 30) };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(240, 240) };
};

class BPMagneticField : public BPTileBasedEntity
{
	DrawInfo draw_info = { "content\\textures\\magnetic_field.png", false, -5, 0 };

	// [Serialize]
	MagneticField magnetic_field = { 5 };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(240, 240) };
};
