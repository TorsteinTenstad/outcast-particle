#include "SFML.h"

SFML::SFML()
{
	util::Platform platform;
	sf::RenderWindow window;
	// in Windows at least, this must be called before creating the window
	float screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
	// Use the screenScalingFactor
	window.create(sf::VideoMode(1920 * screenScalingFactor, 1080 * screenScalingFactor), "outcast-particle");
	platform.setIcon(window.getSystemHandle());

	SystemsManager systems_manager = SystemsManager();
	systems_manager.AddPlayerEntity(0, 0, 0, 0, -10000);
	systems_manager.AddParticleEntity(1920 / 2, 1080 / 2, 10000);

	Timer timer = Timer();
	while (window.isOpen())
	{
		window.clear();
		float dt = timer.GetElapsedSeconds();
		systems_manager.Update(window, dt);
		window.display();
	}
}