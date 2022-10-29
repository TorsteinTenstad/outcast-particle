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
	systems_manager.AddPlayerEntity(1920, 1080, 0, 0, -100);
	systems_manager.AddMovingParticleEntity(1920 / 2 + 300, 1080 / 2 + 300, -100, 100, -100);
	systems_manager.AddMovingParticleEntity(1920 / 2 + 300, 1080 / 2 - 300, 100, 100, -100);
	systems_manager.AddMovingParticleEntity(1920 / 2 - 300, 1080 / 2 + 300, -100, -100, -100);
	systems_manager.AddMovingParticleEntity(1920 / 2 - 300, 1080 / 2 - 300, 100, -100, -100);
	systems_manager.AddParticleEntity(1920 / 2, 1080 / 2, 1000000);
	//for (int x = 0; x < 1920; x += 128)
	{
		//systems_manager.AddParticleEntity(x, 0, -10);
		//systems_manager.AddParticleEntity(x, 1080, -10);
	}
	//for (int y = 0; y < 1080; y += 128)
	{
		//systems_manager.AddParticleEntity(0, y, -10);
		//systems_manager.AddParticleEntity(1920, y, -10);
	}
	systems_manager.SaveEntitiesToFile("oa_test.txt");

	Timer timer = Timer();
	while (window.isOpen())
	{
		window.clear();
		float dt = timer.GetElapsedSeconds();
		systems_manager.Update(window, dt);
		window.display();
	}
}