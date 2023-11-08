
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

#define LEADERBOARD_QUICKEST_WIN "Quickest Win"
#define LEADERBOARD_FEET_TRAVELED "Feet Traveled"
class CLeaderboards
{
public:
	bool pending = false;
	SteamLeaderboard_t steam_leaderboard = 0;
	void OnFindLeaderboard(LeaderboardFindResult_t* pFindLeaderboardResult, bool bIOFailure)
	{
		// see if we encountered an error during the call
		if (!pFindLeaderboardResult->m_bLeaderboardFound || bIOFailure)
		{
			std::cout << "Leaderboard could not be found" << std::endl;
			return;
		}

		// check to see which leaderboard handle we just retrieved
		const char* leaderboard_name = SteamUserStats()->GetLeaderboardName(pFindLeaderboardResult->m_hSteamLeaderboard);
		pending = false;
		std::cout << "Leaderboard name: " << leaderboard_name << std::endl;
		steam_leaderboard = pFindLeaderboardResult->m_hSteamLeaderboard;
	}

	CCallResult<CLeaderboards, LeaderboardFindResult_t> m_SteamCallResultCreateLeaderboard;
	CCallResult<CLeaderboards, LeaderboardScoreUploaded_t> m_SteamCallResultUploadScore;
	CCallResult<CLeaderboards, LeaderboardScoresDownloaded_t> m_callResultDownloadScore;

	void FindLeaderboards()
	{
		SteamAPICall_t hSteamAPICall = 0;
		std::cout << "hSteamAPICall: " << hSteamAPICall << std::endl;

		if (!steam_leaderboard)
		{
			// find/create a leaderboard for the quickest win
			hSteamAPICall = SteamUserStats()->FindOrCreateLeaderboard("TEST_LEADERBOARD",
				k_ELeaderboardSortMethodAscending,
				k_ELeaderboardDisplayTypeTimeSeconds);
		}
		std::cout << "hSteamAPICall: " << hSteamAPICall << std::endl;
		if (hSteamAPICall != 0)
		{
			// set the function to call when this API call has completed
			m_SteamCallResultCreateLeaderboard.Set(hSteamAPICall, this, &CLeaderboards::OnFindLeaderboard);
			pending = true;
		}
	}

	void UpdateScore(int score)
	{
		SteamAPICall_t hSteamAPICall = 0;
		if (steam_leaderboard)
		{
			// the leaderboard handle is valid, upload the score
			hSteamAPICall = SteamUserStats()->UploadLeaderboardScore(steam_leaderboard,
				k_ELeaderboardUploadScoreMethodKeepBest,
				score,
				NULL,
				0);
		}
		if (hSteamAPICall != 0)
		{
			// set the function to call when this API call has completed
			m_SteamCallResultUploadScore.Set(hSteamAPICall, this, &CLeaderboards::OnUploadScore);
			pending = true;
		}
	}

	void OnUploadScore(LeaderboardScoreUploaded_t* pFindLeaderboardResult, bool bIOFailure)
	{
		// see if we encountered an error during the call
		if (!pFindLeaderboardResult->m_bSuccess || bIOFailure)
		{
			std::cout << "Score could not be uploaded" << std::endl;
			return;
		}

		// the score was successfully uploaded
		std::cout << "Score was successfully uploaded to Steam" << std::endl;
		pending = false;
	}

	bool DownloadScores()
	{
		if (!steam_leaderboard) { return false; }

		// load the specified leaderboard data around the current user
		SteamAPICall_t hSteamAPICall = SteamUserStats()->DownloadLeaderboardEntries(
			steam_leaderboard, k_ELeaderboardDataRequestGlobalAroundUser, -4, 5);
		m_callResultDownloadScore.Set(hSteamAPICall, this, &CLeaderboards::OnDownloadScore);
		pending = true;
		return true;
	}

	void OnDownloadScore(LeaderboardScoresDownloaded_t* pResult, bool bIOFailure)
	{
		// see if we encountered an error during the call
		if (bIOFailure)
		{
			std::cout << "Score could not be downloaded" << std::endl;
			return;
		}
		pending = false;
		// the score was successfully downloaded
		std::cout << "Score was successfully downloaded from Steam" << std::endl;

		// loop through all the downloaded entries and print their data
		for (int index = 0; index < pResult->m_cEntryCount; index++)
		{
			LeaderboardEntry_t leaderboardEntry;
			int32 details[1];
			SteamUserStats()->GetDownloadedLeaderboardEntry(pResult->m_hSteamLeaderboardEntries, index, &leaderboardEntry, details, 1);
			std::cout << "Leaderboard entry #" << index << std::endl;
			std::cout << "  score: " << leaderboardEntry.m_nScore << std::endl;
			std::cout << "  steam ID: " << leaderboardEntry.m_steamIDUser.ConvertToUint64() << std::endl;
			std::cout << "  global rank: " << leaderboardEntry.m_nGlobalRank << std::endl;
			std::cout << "  details: " << details[0] << std::endl;
		}
	}
};

int main()
{
	std::cout << "Hello World!\n";

	if (SteamAPI_RestartAppIfNecessary(k_uAppIdInvalid)) // Replace with your App ID
	{
		return 1;
	}

	if (!SteamAPI_Init())
	{
		printf("Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed).\n");
		return 1;
	}

	std::cout << "SteamAPI_Init() succeeded." << std::endl;

	auto l = CLeaderboards();
	l.FindLeaderboards();
	while (l.pending)
	{
		SteamAPI_RunCallbacks();
	}
	l.UpdateScore(100);
	while (l.pending)
	{
		SteamAPI_RunCallbacks();
	}
	l.DownloadScores();
	while (l.pending)
	{
		SteamAPI_RunCallbacks();
	}

	std::srand(std::time(nullptr));
	SFML sfml_app = SFML();
	Game game = Game();
	sfml_app.RunWindow(std::bind(&Game::Update, &game, std::placeholders::_1));
	return 0;
}
