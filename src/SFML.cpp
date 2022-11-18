#include "SFML.h"
#include "game.hpp"

SFML::SFML()
{
	util::Platform platform;
	// in Windows at least, this must be called before creating the window
	float screenScalingFactor = platform.getScreenScalingFactor(globals.render_window.getSystemHandle());
	// Use the screenScalingFactor
	globals.render_window.create(sf::VideoMode(1920 * screenScalingFactor, 1080 * screenScalingFactor), "outcast-particle"); //, sf::Style::Fullscreen);
	//globals.render_window.setFramerateLimit(60);
	platform.setIcon(globals.render_window.getSystemHandle());
}

void SFML::RunWindow(std::function<void(float)> update_func)
{
	Timer timer = Timer();
	while (globals.render_window.isOpen())
	{
		globals.render_window.clear();
		float dt = timer.GetElapsedSeconds();
		std::cout << 1 / dt << "\n";
		for (int i = 0; i < 10; i++)
		{
			update_func(dt / 10);
		}

		globals.render_window.display();
	}
}