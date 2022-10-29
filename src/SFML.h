#include "Platform/Platform.hpp"
#include "game.hpp"
#include "level.hpp"
#include "timer.hpp"

class SFML
{
private:
	sf::RenderWindow window_;

public:
	SFML();
	void RunWindow(Game& game);
};