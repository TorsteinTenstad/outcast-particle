#include "level.hpp"

class BPParticle {
	DrawInfo draw_info = { "content\\particle_100_red+.png" };
	Draggable draggable = {};
	ClickedOn clicked_on = {};

	// [Serialize]
	Charge charge = { 0 };
	// [Serialize]
	Position position = { sf::Vector2f(0, 0) };
	// [Serialize]
	Radius radius = { 0 };
};

class BPMovingParticle : BPParticle {
	DrawInfo draw_info = { "content\\particle_100_blue+.png" };
	Acceleration acceleration = {};
	ReceivedForces received_forces = {};

	// [Serialize]
	Velocity velocity = { sf::Vector2f(0, 0) };
};

class BPPlayer : BPMovingParticle {
	DrawInfo draw_info = { "content\\particle_100_blue+.png" };
  	Player player = {};
};

