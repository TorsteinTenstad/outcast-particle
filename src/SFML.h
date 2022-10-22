#include "Platform/Platform.hpp"
#include "systems_manager.hpp"

class RenderSystem
{
private:
	util::Platform platform_;
	sf::RenderWindow window_;

public:
	RenderSystem();
	Update(float dt);
};

class EventSystem
{
public:
	EventSystem();
	Update(float dt);
}