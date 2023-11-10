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
	if (SteamAPI_RestartAppIfNecessary(k_uAppIdInvalid)) // Replace with your App ID
	{
		return 1;
	}

	if (SteamAPI_Init())
	{
		globals.steam_username = std::string(SteamFriends()->GetPersonaName());
		globals.steam_user_id = SteamUser()->GetSteamID().ConvertToUint64();
	}

	cpr::Response r = cpr::Get(cpr::Url { "http://volatile-particle.deno.dev/" });
	r.status_code;			  // 200
	r.header["content-type"]; // application/json; charset=utf-8
	r.text;					  // JSON text string

	std::srand(std::time(nullptr));
	SFML sfml_app = SFML();
	Game game = Game();
	sfml_app.RunWindow(std::bind(&Game::Update, &game, std::placeholders::_1));
	return 0;
}
