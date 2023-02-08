#include "SFML.hpp"
#include "game.hpp"
#include <chrono>
#include <iostream>
#include <thread>
SFML::SFML()
{
	globals.render_window.create(sf::VideoMode(1280, 720), "outcast-particle");
	//globals.render_window.setFramerateLimit(10);
}

void SFML::RunWindow(std::function<void(float)> update_func)
{
	Timer timer = Timer();
	float seconds_since_last_fps_print = 0;
	int fps = 0;
	while (globals.render_window.isOpen())
	{
		float dt = timer.GetElapsedSeconds();
		globals.time = timer.GetSecondsSinceInit();
		seconds_since_last_fps_print += dt;
		fps++;
		if (seconds_since_last_fps_print > 1)
		{
			std::cout << "FPS: " << fps << "\n";
			fps = 0;
			seconds_since_last_fps_print = 0;
		}
		update_func(dt);
	}
}