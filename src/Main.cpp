#include "SFML.h"
#include "game.hpp"
#include "globals.hpp"

static Globals globals_;
Globals& globals = globals_;

int main()
{
	SFML sfml_app = SFML();
	Game game = Game();
	game.InitSystems(); 
	sfml_app.RunWindow(game);
	return 0;
}
