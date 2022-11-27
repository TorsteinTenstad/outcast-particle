#include "SFML.h"
#include "game.hpp"
#include "globals.hpp"
#include <functional>
using namespace std::placeholders;

static Globals globals_;
Globals& globals = globals_;

int main()
{
	SFML sfml_app = SFML();
	Game game = Game();
	game.Init();
	sfml_app.RunWindow(std::bind(&Game::Update, &game, _1));
	return 0;
}