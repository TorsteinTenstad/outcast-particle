#include "SFML.h"
#include "game.hpp"
#include "globals.hpp"

static Globals globals_;
Globals& globals = globals_;

int main()
{
	Game game = Game();
	SFML sfml_app = SFML();
	sfml_app.RunWindow(game);
	return 0;
}
