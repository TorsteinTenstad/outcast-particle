#pragma once
#include "PCH.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utilityfunctions.hpp"

static float Angle(sf::Vector2f v)
{
	return std::atan2(v.y, v.x);
}

class DisplayVelocitySystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		(void)cursor_and_keys;
		(void)dt;
		std::map<int, Position>& position_map = level.position_;
		std::map<int, Velocity>& velocity_map = level.velocity_;

		for (auto const& [entity_id, velocity] : velocity_map)
		{
			float indicator_lenght = Magnitude(velocity.velocity) / 4;
			float indicator_width = 10;
			sf::RectangleShape indicator = sf::RectangleShape(sf::Vector2f(indicator_lenght, indicator_width));
			indicator.setOrigin(0, indicator_width / 2);
			indicator.setRotation(Angle(velocity.velocity) * (180 / PI));
			indicator.setPosition(position_map[entity_id].position);
			globals.render_window.draw(indicator);
		}
	}
};