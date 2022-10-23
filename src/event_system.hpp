#pragma once
#include "PCH.hpp"
#include "physics_components.hpp"

class EventSystem
{
public:
	void Update(sf::RenderWindow& window, std::map<int, Acceleration>& acceleration_map)
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
				if (event.key.code == sf::Keyboard::A)
				{
					acceleration_map[1].acceleration.x = -300;
				}
				if (event.key.code == sf::Keyboard::D)
				{
					acceleration_map[1].acceleration.x = 300;
				}
				if (event.key.code == sf::Keyboard::W)
				{
					acceleration_map[1].acceleration.y = -300;
				}
				if (event.key.code == sf::Keyboard::S)
				{
					acceleration_map[1].acceleration.y = 300;
				}
			}
			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::A)
				{
					acceleration_map[1].acceleration.x = -0;
				}
				if (event.key.code == sf::Keyboard::D)
				{
					acceleration_map[1].acceleration.x = 0;
				}
				if (event.key.code == sf::Keyboard::W)
				{
					acceleration_map[1].acceleration.y = -0;
				}
				if (event.key.code == sf::Keyboard::S)
				{
					acceleration_map[1].acceleration.y = 0;
				}
			}
		}
	}
};