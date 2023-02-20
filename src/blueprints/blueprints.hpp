#include "level.hpp"
class BPTexturedRectangle
{
	DrawPriority draw_priority = { 0 };
	DrawInfo draw_info = { "_", false, 0 };
	WidthAndHeight width_and_height = { sf::Vector2f(120, 120) };
	Position position = {};
};
class BPButton
{
	ReceivesButtonEvents can_receive_press = {};
	DrawPriority draw_priority = { 100 };
	DrawInfo draw_info = { "content\\textures\\white.png", false, 0 };
	FillColor fill_color = {};
	MouseInteractionDependentFillColor mouse_interaction_dependent_fill_color = { sf::Color(200, 200, 200), sf::Color(120, 120, 120), sf::Color(150, 150, 150) };
	Shader shader = { "", "shaders\\round_corners.frag", {}, {}, {} };
	Text text = {};
	WidthAndHeight width_and_height = { sf::Vector2f(10, 2) * 120.f };
	Position position = {};
};

class BPMenuNavigationButton : public BPButton
{
	OnReleasedThisFrame on_released_this_frame = {};
	MenuNavigatable menu_navigatable = {};
};

class BPMenuNavigator
{
	DrawInfo draw_info = { "content\\textures\\menu_navigator.png", true, 0 };
	WidthAndHeight width_and_height = { sf::Vector2f(120, 180) };
	FillColor fill_color = { sf::Color(120, 120, 120) };
	DrawPriority draw_priority = { 101 };
	MenuNavigator menu_navigator = {};
	Position position = { sf::Vector2f(0, 0) };
};

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
	Radius radius = { 120 };

	Acceleration acceleration = {};
	ReceivedForces received_forces = {};
	Intersection intersection = {};
	Collision collision = {};
	Children children = {};
	Trail trail = {};
	DrawPriority draw_priority = { 8 };
	Shader shader = { "shaders\\player.vert", "shaders\\player.frag", {}, {}, {} };
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

class BPLaser : public BPEditableEntity
{
	DrawInfo draw_info = { "content\\textures\\laser_horisontal.png", false, 0 };
	DrawPriority draw_priority = { 3 };
	OrientationDependentDrawInfo orientation_dependent_draw_info = {};
	Editable editable = { 60 };
	KillOnIntersection kill_on_intersection = {};
	Shader shader = { "shaders\\test.vert", "", {}, {}, {} };
	SoundInfo sound_info = { "content\\sounds\\laser.wav" };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(120, 60) };
};

class BPCoin : public BPEditableEntity
{
	DrawInfo draw_info = { "content\\textures\\coin.png", true, 0 };
	DrawPriority draw_priority = { 3 };
	Editable editable = { 120 };
	Coin coin = {};
	SegmentedGlowEffect segmented_glow_effect = {};
	Radius radius = { 60 };
	SoundInfo sound_info = { "content\\sounds\\coin.wav" };
};

class BPWall : public BPEditableEntity
{
	DrawInfo draw_info = { "content\\textures\\block.png", false, 0 };
	DrawPriority draw_priority = { 4 };
	SoundInfo sound_info = { "content\\sounds\\thud.wav" };
	Collision collision = { 0.2, 75 };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(120, 120) };
};

class BPBounceWall : public BPWall
{
	DrawInfo draw_info = { "content\\textures\\block_green.png", false, 0 };
	Collision collision = { 1, 75 };
};

class BPNoBounceWall : public BPWall
{
	DrawInfo draw_info = { "content\\textures\\block_dark.png", false, 0 };
	Collision collision = { 0.05, 75 };
};

class BPGoal : public BPEditableEntity
{
	DrawInfo draw_info = { "content\\textures\\goal.png", false, 0 };
	DrawPriority draw_priority = { 2 };
	Goal goal = {};
	SoundInfo sound_info = { "content\\sounds\\happy_transition.wav", false, 1 };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(240, 240) };
};

class BPElectricField : public BPEditableEntity
{
	DrawInfo draw_info = { "content\\textures\\electric_field.png", false, 0 };
	DrawPriority draw_priority = { 1 };

	// [Serialize]
	ElectricField electric_field = { sf::Vector2f(0, 0.25) };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(120, 120) };
};

class BPMagneticField : public BPEditableEntity
{
	DrawInfo draw_info = { "content\\textures\\magnetic_field.png", false, 0 };
	DrawPriority draw_priority = { 1 };

	// [Serialize]
	MagneticField magnetic_field = { 0.1 };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(120, 120) };
};

class BPTextPopupSpawner : public BPEditableEntity
{
	DrawInfo draw_info = { "content\\textures\\transparent.png", false, 0 };
	DrawPriority draw_priority = { 2 };

	// [Serialize]
	WidthAndHeight width_and_height = { sf::Vector2f(120, 120) };

	// [Serialize]
	TextPopupSpawner text_popup_spawner = { "ipsum lorem" };
};

class BPText
{
	Text text = {};
	DrawPriority draw_priority = { 101 };
	Shader shader = { "", "shaders\\scroll.frag", {}, {}, {} };
	Position position = {};
};
