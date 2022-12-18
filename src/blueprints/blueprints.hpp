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
	DrawInfo draw_info = { "_", false, 0 };
	DrawPriority draw_priority = { 5 };
	ChargeDependentDrawInfo charge_dependent_draw_info = {};
	Editable editable = { true, true, false, 0, sf::Vector2f(0, 0), false };
	Radius radius = { 120 };

	// [Serialize]
	Charge charge = { 10000 };
};

class BPMovingParticle : public BPStaticParticle
{
	DrawPriority draw_priority = { 6 };
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
	DrawPriority draw_priority = { 7 };
	Shader shader = { "", "shaders\\scale_animation.frag", { { "time", 1 } } };

	// [Serialize]
	Player player = { true, true, 1000 };
};

class BPLaser : public BPEntity
{
	DrawInfo draw_info = { "_", false, 0 };
	DrawPriority draw_priority = { 3 };
	OrientationDependentDrawInfo orientation_dependent_draw_info = { "content\\textures\\laser_horisontal.png", "content\\textures\\laser_vertical.png" };
	Editable editable = { true, false, false, 60, sf::Vector2f(0, 0), false };
	KillOnIntersection kill_on_intersection = {};
	Shader shader = { "shaders\\test.vert", "", { { "_time", 1 } } };
	SoundInfo sound_info = { "content\\sounds\\laser.wav" };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(120, 60) };
};

class BPWall : public BPTileBasedEntity
{
	DrawInfo draw_info = { "content\\textures\\block.png", false, 0 };
	DrawPriority draw_priority = { 4 };
	SoundInfo sound_info = { "content\\sounds\\thud.wav" };
	Collision collision = { 0.2, 75 };
};

class BPGoal : public BPTileBasedEntity
{
	DrawInfo draw_info = { "content\\textures\\goal.png", false, 0 };
	DrawPriority draw_priority = { 2 };
	Goal goal = {};
	KillOnIntersection kill_on_intersection = {};
	SoundInfo sound_info = { "content\\sounds\\fanfare.wav" };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(240, 240) };
};

class BPElectricField : public BPTileBasedEntity
{
	DrawInfo draw_info = { "content\\textures\\electric_field.png", false, 0 };
	DrawPriority draw_priority = { 1 };

	// [Serialize]
	ElectricField electric_field = { sf::Vector2f(0, 0.25) };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(240, 240) };
};

class BPMagneticField : public BPTileBasedEntity
{
	DrawInfo draw_info = { "content\\textures\\magnetic_field.png", false, 0 };
	DrawPriority draw_priority = { 1 };

	// [Serialize]
	MagneticField magnetic_field = { 0.1 };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(240, 240) };
};
