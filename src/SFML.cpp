#include "SFML.hpp"
#include "SFML/System/Clock.hpp"
#include "game.hpp"
#include <chrono>
#include <iostream>
#include <thread>

SFML::SFML()
{
}

void SFML::RunWindow(std::function<void(float)> update_func)
{
	sf::Clock frame_clock = sf::Clock();
	float seconds_since_last_fps_print = 0;
	int fps = 0;
	float worst_frame_time = 0;
	while (globals.render_window.isOpen())
	{
		float dt = frame_clock.getElapsedTime().asSeconds();
		frame_clock.restart();
		seconds_since_last_fps_print += dt;
		fps++;
		worst_frame_time = std::max(worst_frame_time, dt);
		if (seconds_since_last_fps_print > 1)
		{
			std::cout << "FPS: " << fps << ", worst frame time: " << int(1000 * worst_frame_time) << " ms\n";
			fps = 0;
			worst_frame_time = 0;
			seconds_since_last_fps_print = 0;
		}
		update_func(dt);
	}
}