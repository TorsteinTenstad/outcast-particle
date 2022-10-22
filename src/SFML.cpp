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

	sf::Event event;
	while (window.isOpen())
	{
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
		}

		window.clear();
		float dt = 0.5;
		systems_manager.Update(window, dt);
		window.display();
	}
}