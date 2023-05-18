#include "SFML.hpp"
#include "game.hpp"
#include "globals.hpp"
#include <cstdlib>
#include <ctime>
#include <functional>

static Globals globals_;
Globals& globals = globals_;

int main()
{
	std::srand(std::time(nullptr));
	SFML sfml_app = SFML();
	Game game = Game();
	sfml_app.RunWindow(std::bind(&Game::Update, &game, std::placeholders::_1));
	return 0;
}
