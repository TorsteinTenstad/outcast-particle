#include "level.hpp"

class BPEntity
{
	CanReceivePress can_receive_press = {};
	Editable editable = {};
	// [Serialize]
	Position position = { sf::Vector2f(0, 0) };
};

class BPTileBasedEntity : public BPEntity
{
	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(120, 120) };
};

class BPStaticParticle : public BPEntity
{
	DrawInfo draw_info = { "_", false, 0 };
	DrawPriority draw_priority = { 6 };
	ChargeDependentDrawInfo charge_dependent_draw_info = {};

	Radius radius = { 120 };

	// [Serialize]
	Charge charge = { 10000 };
};

class BPMovingParticle : public BPStaticParticle
{
	DrawPriority draw_priority = { 7 };

	Acceleration acceleration = {};
	ReceivedForces received_forces = {};
	Intersection intersection = {};
	Collision collision = {};
	Trail trail = {};

	// [Serialize]
	Velocity velocity = { sf::Vector2f(0, 0) };
};

class BPPlayer : public BPEntity
{
	DrawInfo draw_info = { "_", false, 0 };
	Radius radius = { 120 };

	Acceleration acceleration = {};
	ReceivedForces received_forces = {};
	Intersection intersection = {};
	Collision collision = {};
	Children children = {};
	Trail trail = {};
	DrawPriority draw_priority = { 8 };
	Shader shader = { "shaders\\player.vert", "shaders\\player.frag", {}, { { "_time", 0.f } }, {} };
	SoundInfo sound_info = { "content\\sounds\\wav.wav" };
	Face face = {};
	ForceVisualization force_visualization = {};
	PlayerBehaviors player_behaviours = {};

	// [Serialize]
	Charge charge = { 10000 };
	// [Serialize]
	Velocity velocity = { sf::Vector2f(0, 0) };
	// [Serialize]
	Player player = { true, true, 1000 };
};

class BPLaser : public BPEntity
{
	DrawInfo draw_info = { "content\\textures\\laser_horisontal.png", false, 0 };
	DrawPriority draw_priority = { 3 };
	OrientationDependentDrawInfo orientation_dependent_draw_info = {};
	Editable editable = { 60 };
	KillOnIntersection kill_on_intersection = {};
	Shader shader = { "shaders\\test.vert", "", {}, { { "_time", 0.f } }, {} };
	SoundInfo sound_info = { "content\\sounds\\laser.wav" };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(120, 60) };
};

class BPCoin : public BPEntity
{
	DrawInfo draw_info = { "content\\textures\\coin.png", true, 0 };
	DrawPriority draw_priority = { 3 };
	Editable editable = { 120 };
	Coin coin = {};
	SegmentedGlowEffect segmented_glow_effect = {};
	Radius radius = { 60 };
	SoundInfo sound_info = { "content\\sounds\\coin.wav" };
};

class BPButton : BPEntity
{
	DrawPriority draw_priority = { 100 };
	Button button = {};
	WidthAndHeight width_and_height = { sf::Vector2f(3072, 432) };
	DrawInfo draw_info = { "content\\textures_generated\\button_menu_wide.png", true, 0 };
	Text text = {};
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
	SoundInfo sound_info = { "content\\sounds\\happy_transition.wav", false, 1 };

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

class BPTextPopupSpawner : public BPTileBasedEntity
{
	DrawInfo draw_info = { "_", false, 0 };
	DrawPriority draw_priority = { 2 };

	// [Serialize]
	TextPopupSpawner text_popup_spawner = { "ipsum lorem" };
};
