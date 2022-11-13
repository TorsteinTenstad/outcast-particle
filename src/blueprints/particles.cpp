#include "level.hpp"

class BPStaticParticle
{
	ChargeDependentDrawInfo charge_dependent_draw_info = { "content\\particle_100_red+.png", "content\\particle_100_green.png", "content\\particle_100_green-.png" };
	Editable editable = { true, false, false, sf::Vector2f(0, 0), false };
	ClickedOn clicked_on = {};
	Radius radius = { 50 };

	// [Serialize]
	Charge charge = { 0 };
	// [Serialize]
	Position position = { sf::Vector2f(0, 0) };
};

class BPMovingParticle : public BPStaticParticle
{
	Acceleration acceleration = {};
	ReceivedForces received_forces = {};
	Editable editable = { true, true, false, sf::Vector2f(0, 0), false };

	// [Serialize]
	Velocity velocity = { sf::Vector2f(0, 0) };
};

class BPPlayer : public BPMovingParticle
{
	ChargeDependentDrawInfo charge_dependent_draw_info = { "content\\particle_100_blue+.png", "content\\particle_100_blue.png", "content\\particle_100_blue-.png" };
	Player player = {};
};
