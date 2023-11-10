
#include "SFML.hpp"
#include "game.hpp"
#include "globals.hpp"
#include "steam/steam_api.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <functional>

static Globals globals_;
Globals& globals = globals_;

int main()
{
	if (SteamAPI_RestartAppIfNecessary(k_uAppIdInvalid)) // Replace with your App ID
	{
		return 1;
	}

	if (!SteamAPI_Init())
	{
		printf("Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed).\n");
		return 1;
	}

	std::srand(std::time(nullptr));
	SFML sfml_app = SFML();
	Game game = Game();
	sfml_app.RunWindow(std::bind(&Game::Update, &game, std::placeholders::_1));
	return 0;
}
