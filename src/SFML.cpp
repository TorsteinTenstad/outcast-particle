#include "SFML.h"
#include "game.hpp"
#include <chrono>
#include <thread>
SFML::SFML()
{
	util::Platform platform;
	// in Windows at least, this must be called before creating the window
	float screenScalingFactor = platform.getScreenScalingFactor(globals.render_window.getSystemHandle());
	// Use the screenScalingFactor
	globals.render_window.create(sf::VideoMode(1280, 720), "outcast-particle");
	//globals.render_window.create(sf::VideoMode(), "outcast-particle", sf::Style::Fullscreen);
	//globals.render_window.setFramerateLimit(60);
	platform.setIcon(globals.render_window.getSystemHandle());
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
		globals.render_window.display();
	}
}