#include "SFML.h"
#include "game.hpp"

SFML::SFML()
{
	util::Platform platform;
	// in Windows at least, this must be called before creating the window
	float screenScalingFactor = platform.getScreenScalingFactor(window_.getSystemHandle());
	// Use the screenScalingFactor
	window_.create(sf::VideoMode(1920 * screenScalingFactor, 1080 * screenScalingFactor), "outcast-particle");
	window_.setFramerateLimit(60);
	platform.setIcon(window_.getSystemHandle());
}

void SFML::RunWindow(Game& game)
{
	Timer timer = Timer();
	while (window_.isOpen())
	{
		window_.clear();
		float dt = timer.GetElapsedSeconds();
		game.Update(window_, dt);
		window_.display();
	}
}