#pragma once
#include "PCH.hpp"
#include "comp_physics.hpp"
#include "globals.hpp"
#include "utilityfunctions.hpp"

static float Angle(sf::Vector2f v)
{
	return std::atan2(v.y, v.x) * (180 / PI);
}

class DisplayVelocitySystem
{
public:
	void Update(std::map<int, Position>& position_map, std::map<int, Velocity>& velocity_map)
	{
		for (auto const& [entity_id, velocity] : velocity_map)
		{
			float indicator_lenght = Magnitude(velocity.velocity) / 4;
			float indicator_width = 10;
			sf::RectangleShape indicator = sf::RectangleShape(sf::Vector2f(indicator_lenght, indicator_width));
			indicator.setOrigin(0, indicator_width / 2);
			indicator.setRotation(Angle(velocity.velocity));
			indicator.setPosition(position_map[entity_id].position);
			globals.render_window.draw(indicator);
		}
	}
};