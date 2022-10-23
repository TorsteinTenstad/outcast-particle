#pragma once
#include "PCH.hpp"
#include "affected_by_keyboard_forces_component.hpp"
#include "physics_components.hpp"
#include "recieved_forces_component.hpp"

class EventSystem
{
public:
	void Update(sf::RenderWindow& window, std::map<int, AffectedByKeyboardForces>& affected_by_keyboard_forces_map, std::map<int, RecievedForces>& recieved_forces_map)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::Resized)
			{
				// update the view to the new size of the window
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
			}
			if (event.type == sf::Event::KeyPressed)
			{
				for (auto& [entity_id, _] : affected_by_keyboard_forces_map)
				{
					(void)_;
					float force_magnitude = 300;
					if (event.key.code == sf::Keyboard::A)
					{
						recieved_forces_map[entity_id].keyboard_force.x = -force_magnitude;
					}
					if (event.key.code == sf::Keyboard::D)
					{
						recieved_forces_map[entity_id].keyboard_force.x = force_magnitude;
					}
					if (event.key.code == sf::Keyboard::W)
					{
						recieved_forces_map[entity_id].keyboard_force.y = -force_magnitude;
					}
					if (event.key.code == sf::Keyboard::S)
					{
						recieved_forces_map[entity_id].keyboard_force.y = force_magnitude;
					}
				}
			}

			if (event.type == sf::Event::KeyPressed)
			{
				for (auto& [entity_id, _] : affected_by_keyboard_forces_map)
				{
					(void)_;
					float force_magnitude = 0;
					if (event.key.code == sf::Keyboard::A)
					{
						recieved_forces_map[entity_id].keyboard_force.x = -force_magnitude;
					}
					if (event.key.code == sf::Keyboard::D)
					{
						recieved_forces_map[entity_id].keyboard_force.x = force_magnitude;
					}
					if (event.key.code == sf::Keyboard::W)
					{
						recieved_forces_map[entity_id].keyboard_force.y = -force_magnitude;
					}
					if (event.key.code == sf::Keyboard::S)
					{
						recieved_forces_map[entity_id].keyboard_force.y = force_magnitude;
					}
				}
			}
		}
	}
};