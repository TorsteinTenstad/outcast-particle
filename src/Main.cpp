#include "SFML.h"
#include "game.hpp"

int main()
{
	Game game = Game();
	SFML sfml_app = SFML();
	sfml_app.RunWindow(game);
	return 0;
}
