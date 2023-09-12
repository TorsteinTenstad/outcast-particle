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
	Shader shader = { "", "SHADERS_DIR\\particle.frag", {}, {}, {} };
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

	// [Serialize]
	Velocity velocity = { sf::Vector2f(0, 0) };
};

class BPBlackHole : public BPEditableEntity
{
	DrawInfo draw_info = {};
	DrawPriority draw_priority = { 1 };
	Shader shader = { "", "SHADERS_DIR\\black_hole.frag", {}, {}, {} };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(240, 240) };
};

class BPPlayer : public BPEditableEntity
{
	DrawInfo draw_info = {};
	Radius radius = { 108 };

	Acceleration acceleration = {};
	ReceivedForces received_forces = {};
	Trail trail = {};
	Mass mass = {};
	Intersection intersection = {};
	Collision collision = {};
	Children children = {};
	Trail trail = {};
	DrawPriority draw_priority = { 12 };
	Shader shader = { "SHADERS_DIR\\player.vert", "SHADERS_DIR\\particle.frag", {}, {}, {} };
	SoundInfo sound_info = { { { DEFAULT, "SOUNDS_DIR\\wav.wav" }, { TO_NEUTRAL, "SOUNDS_DIR\\to_neutral.wav" }, { FROM_NEUTRAL, "SOUNDS_DIR\\from_neutral.wav" } } };
	Face face = {};
	ForceVisualization force_visualization = {};
	PlayerBehaviors player_behaviors = {};

	// [Serialize]
	Charge charge = { 10000 };

	// [Serialize]
	Velocity velocity = { sf::Vector2f(0, 0) };

	// [Serialize]
	Player player = {};
};

class BPLaser : public BPEditableEntity
{
	Laser laser = {};
	DrawPriority draw_priority = { 3 };
	Editable editable = { 60 };
	FillColor fill_color = {};

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(120, 60) };
	// [Serialize]
	TogglingLaser toggling_laser = {};
};

class BPCoin : public BPEditableEntity
{
	DrawPriority draw_priority = { 5 };
	Coin coin = {};
	SegmentedGlowEffect segmented_glow_effect = {};
	Children children = {};
	Radius radius = { 60 };
	SoundInfo sound_info = { { { DEFAULT, "SOUNDS_DIR\\coin.wav" } } };
};

class BPWall : public BPEditableEntity
{
	Wall wall = {};
	DrawPriority draw_priority = { 4 };
	SoundInfo sound_info = { { { DEFAULT, "SOUNDS_DIR\\wall_3.wav" } } };

	// [Serialize]
	Collision collision = { 0.2, 75 };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(120, 120) };
};

class BPGoal : public BPEditableEntity
{
	DrawInfo draw_info = {};
	Shader shader = { "", "SHADERS_DIR\\wormhole.frag", {}, {}, {} };
	DrawPriority draw_priority = { 2 };
	Goal goal = {};
	Wormhole wormhole = {};
	Mass mass = {};
	SoundInfo sound_info = { { { DEFAULT, "SOUNDS_DIR\\happy_transition.wav" } } };
	Radius Radius = { 240 };
};

class BPElectricField : public BPEditableEntity
{
	DrawInfo draw_info = {};
	DrawPriority draw_priority = { 1 };
	Shader shader = { "", "SHADERS_DIR\\electric_field.frag", {}, {}, {} };

	// [Serialize]
	ElectricField electric_field = { sf::Vector2f(0, 0.25) };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(240, 240) };
};

class BPMagneticField : public BPEditableEntity
{
	DrawInfo draw_info = {};
	DrawPriority draw_priority = { 1 };
	Shader shader = { "", "SHADERS_DIR\\magnetic_field.frag", {}, {}, {} };

	// [Serialize]
	MagneticField magnetic_field = { 0.1 };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(240, 240) };
};

class BPTextPopupSpawner : public BPEditableEntity
{
	DrawInfo draw_info = { "TEXTURES_DIR\\transparent.png", false, 0 };
	DrawPriority draw_priority = { 2 };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(120, 120) };

	// [Serialize]
	TextPopupSpawner text_popup_spawner = { "ipsum lorem" };
};