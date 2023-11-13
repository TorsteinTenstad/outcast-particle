#include "SFML.hpp"
#include "game.hpp"
#include "globals.hpp"
#include "steam/steam_api.h"
#include <cpr/cpr.h>
#include <cstdlib>
#include <ctime>
#include <functional>

static Globals globals_;
Globals& globals = globals_;

int main()
{
	if (SteamAPI_RestartAppIfNecessary(2420710)) // Replace with your App ID
	{
		return 1;
	}

	if (SteamAPI_Init())
	{
		globals.steam_username = std::string(SteamFriends()->GetPersonaName());
		globals.steam_user_id = SteamUser()->GetSteamID().ConvertToUint64();
	}
	else
	{
		std::ifstream fallback_username_file("steam_username_fallback.txt");
		std::ifstream fallback_user_id_file("steam_user_id_fallback.txt");
		if (fallback_user_id_file.is_open())
		{
			std::string line;
			std::getline(fallback_user_id_file, line);
			globals.steam_username = line;
			globals.steam_user_id = std::stoull(line);
		}
		if (fallback_username_file.is_open())
		{
			std::string line;
			std::getline(fallback_username_file, line);
			globals.steam_username = line;
			if (!globals.steam_user_id.has_value())
			{
				globals.steam_user_id = 0;
				for (char c : globals.steam_username.value())
				{
					globals.steam_user_id.value() += c;
				}
			}
		}
	}

	std::srand(std::time(nullptr));
	SFML sfml_app = SFML();
	Game game = Game();
	sfml_app.RunWindow(std::bind(&Game::Update, &game, std::placeholders::_1));
	return 0;
}
