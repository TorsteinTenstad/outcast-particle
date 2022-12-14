#include "SFML.hpp"
#include "game.hpp"
#include "globals.hpp"
#include <functional>

static Globals globals_;
Globals& globals = globals_;

int main()
{
	SFML sfml_app = SFML();
	Game game = Game();
	game.Init();
	sfml_app.RunWindow(std::bind(&Game::Update, &game, std::placeholders::_1));
	return 0;
}
