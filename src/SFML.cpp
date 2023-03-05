#include "SFML.hpp"
#include "SFML/System/Clock.hpp"
#include "game.hpp"
#include <chrono>
#include <iostream>
#include <thread>

SFML::SFML()
{
	globals.render_window.create(sf::VideoMode(1280, 720), "outcast-particle");
}

void SFML::RunWindow(std::function<void(float)> update_func)
{
	sf::Clock absolute_clock = sf::Clock();
	sf::Clock frame_clock = sf::Clock();
	float seconds_since_last_fps_print = 0;
	int fps = 0;
	float worst_fps = 1000000000;
	while (globals.render_window.isOpen())
	{
		float dt = frame_clock.getElapsedTime().asSeconds();
		frame_clock.restart();
		globals.time = absolute_clock.getElapsedTime().asSeconds();
		seconds_since_last_fps_print += dt;
		fps++;
		worst_fps = std::min(worst_fps, 1.f / dt);
		if (seconds_since_last_fps_print > 1)
		{
			std::cout << "FPS: " << fps << ", worst frame:" << worst_fps << "\n";
			fps = 0;
			worst_fps = 1000000000;
			seconds_since_last_fps_print = 0;
		}
		update_func(dt);
	}
}