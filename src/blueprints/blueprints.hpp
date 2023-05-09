#include "components/_used_in_serialization.hpp"

class BPEditableEntity
{
	ReceivesButtonEvents can_receive_press = {};
	Editable editable = {};

	// [Serialize]
	Position position = { sf::Vector2f(0, 0) };
};

class BPStaticParticle : public BPEditableEntity
{
	DrawInfo draw_info = {};
	Shader shader = { "", "shaders\\particle.frag", {}, {}, {} };
	DrawPriority draw_priority = { 10 };
	ChargeDependentDrawInfo charge_dependent_draw_info = {};
	Radius radius = { 108 };

	// [Serialize]
	Charge charge = { 10000 };
};

class BPMovingParticle : public BPStaticParticle
{
	DrawPriority draw_priority = { 11 };

	Acceleration acceleration = {};
	ReceivedForces received_forces = {};
	Mass mass = {};
	Intersection intersection = {};
	Collision collision = {};
	Trail trail = {};

	// [Serialize]
	Velocity velocity = { sf::Vector2f(0, 0) };
};

class BPBlackHole : public BPEditableEntity
{
	DrawInfo draw_info = {};
	DrawPriority draw_priority = { 1 };
	Shader shader = { "", "shaders\\black_hole.frag", {}, {}, {} };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(240, 240) };
};

class BPPlayer : public BPEditableEntity
{
	DrawInfo draw_info = {};
	Radius radius = { 108 };

	Acceleration acceleration = {};
	ReceivedForces received_forces = {};
	Mass mass = {};
	Intersection intersection = {};
	Collision collision = {};
	Children children = {};
	Trail trail = {};
	DrawPriority draw_priority = { 12 };
	Shader shader = { "shaders\\player.vert", "shaders\\particle.frag", {}, {}, {} };
	SoundInfo sound_info = { "content\\sounds\\wav.wav" };
	Face face = {};
	ForceVisualization force_visualization = {};
	PlayerBehaviors player_behaviors = {};

	// [Serialize]
	Charge charge = { 10000 };

	// [Serialize]
	Velocity velocity = { sf::Vector2f(0, 0) };

	// [Serialize]
	Player player = { true, true, 1000 };
};

class BPLaser : public BPEditableEntity
{
	Laser laser = {};
	DrawPriority draw_priority = { 3 };
	OrientationDependentDrawInfo orientation_dependent_draw_info = {};
	Editable editable = { 60 };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(120, 60) };
};

class BPCoin : public BPEditableEntity
{
	DrawPriority draw_priority = { 5 };
	Coin coin = {};
	SegmentedGlowEffect segmented_glow_effect = {};
	Children children = {};
	Radius radius = { 60 };
	SoundInfo sound_info = { "content\\sounds\\coin.wav" };
};

class BPWall : public BPEditableEntity
{
	Wall wall = {};
	DrawPriority draw_priority = { 4 };

	// [Serialize]
	SoundInfo sound_info = { "content\\sounds\\thud.wav" };

	// [Serialize]
	Collision collision = { 0.2, 75 };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(120, 120) };
};

class BPGoal : public BPEditableEntity
{
	DrawInfo draw_info = {};
	Shader shader = { "", "shaders\\wormhole.frag", {}, {}, {} };
	DrawPriority draw_priority = { 2 };
	Goal goal = {};
	Wormhole wormhole = {};
	Mass mass = {};
	SoundInfo sound_info = { "content\\sounds\\happy_transition.wav", false, 1 };
	Radius Radius = { 240 };
};

class BPElectricField : public BPEditableEntity
{
	DrawInfo draw_info = {};
	DrawPriority draw_priority = { 1 };
	Shader shader = { "", "shaders\\electric_field.frag", {}, {}, {} };

	// [Serialize]
	ElectricField electric_field = { sf::Vector2f(0, 0.25) };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(240, 240) };
};

class BPMagneticField : public BPEditableEntity
{
	DrawInfo draw_info = {};
	DrawPriority draw_priority = { 1 };
	Shader shader = { "", "shaders\\magnetic_field.frag", {}, {}, {} };

	// [Serialize]
	MagneticField magnetic_field = { 0.1 };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(240, 240) };
};

class BPTextPopupSpawner : public BPEditableEntity
{
	DrawInfo draw_info = {};
	DrawPriority draw_priority = { 2 };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(120, 120) };

	// [Serialize]
	TextPopupSpawner text_popup_spawner = { "ipsum lorem" };
};