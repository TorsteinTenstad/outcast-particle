#pragma once
#include "PCH.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utils.hpp"

static float Angle(sf::Vector2f v)
{
	return std::atan2(v.y, v.x);
}

class DisplayVelocitySystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		if (mode_ != EDIT_MODE)
		{
			return;
		}
		auto& position_map = level.GetComponent<Position>();
		auto& velocity_map = level.GetComponent<Velocity>();

		for (auto const& [entity_id, velocity] : velocity_map)
		{
			float indicator_length = Magnitude(velocity.velocity) / 4;
			float indicator_width = 10;
			sf::RectangleShape indicator = sf::RectangleShape(sf::Vector2f(indicator_length, indicator_width));
			indicator.setOrigin(0, indicator_width / 2);
			indicator.setRotation(Angle(velocity.velocity) * (180 / PI));
			indicator.setPosition(position_map[entity_id].position);
			globals.render_window.draw(indicator);
		}
	}
};