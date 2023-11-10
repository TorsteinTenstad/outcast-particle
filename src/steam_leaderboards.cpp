#include "steam_leaderboards.h"
#include "globals.hpp"
#include "steam/steam_api.h"
#include "utils/container_operations.hpp"
#include "utils/level_id.hpp"
#include <cassert>
#include <iostream>

SteamLeaderboards::SteamLeaderboards(const RecordsManager& records_manager)
{
	for (int i = 0; i < 8; ++i)
	{
		auto group_display_name = "Lab " + std::to_string(i);
		leaderboards_.emplace(group_display_name, std::nullopt);
	}
}

void SteamLeaderboards::Update()
{
	SteamAPI_RunCallbacks();

	if (!find_leaderboard_pending)
	{
		auto predicate = [](const Leaderboard& leaderboard) {
			return !leaderboard.steam_leaderboard_handle.has_value();
		};
		auto next_leaderboard_it = std::find_if(leaderboards_.begin(), leaderboards_.end(), predicate);
		if (next_leaderboard_it != leaderboards_.end())
		{
			find_leaderboard_pending = true;
			SteamAPICall_t steam_api_call = SteamUserStats()->FindLeaderboard(next_leaderboard_it->first.c_str());
			find_leaderboard_call_result_.Set(steam_api_call, this, &SteamLeaderboards::OnLeaderboardFound);
		}
	}

	if (!download_pending)
	{
		auto predicate = [](const Leaderboard& leaderboard) {
			return leaderboard.steam_leaderboard_handle.has_value()
				   && !leaderboard.downloaded_data.has_value();
		};
		auto next_leaderboard_it = std::find_if(leaderboards_.begin(), leaderboards_.end(), predicate);
		if (next_leaderboard_it != leaderboards_.end())
		{
			download_pending = true;
			SteamAPICall_t steam_api_call = SteamUserStats()->DownloadLeaderboardEntries(
				next_leaderboard_it->second.steam_leaderboard_handle.value(),
				k_ELeaderboardDataRequestGlobalAroundUser,
				0,
				0);
			download_call_result_.Set(steam_api_call, this, &SteamLeaderboards::OnDownloaded);
		}
	}
}

void SteamLeaderboards::OnLeaderboardFound(LeaderboardFindResult_t* result, bool io_failure)
{
	find_leaderboard_pending = false;
	if (!result->m_bLeaderboardFound || io_failure)
	{
		std::cout << "Failed to find leaderboard" << std::endl;
		return;
	}
	std::string leaderboard_name = std::string(SteamUserStats()->GetLeaderboardName(result->m_hSteamLeaderboard));
	auto leaderboard_it = leaderboards_.find(leaderboard_name);
	if (leaderboard_it == leaderboards_.end())
	{
		assert(false);
		return;
	}
	leaderboard_it->second.steam_leaderboard_handle = result->m_hSteamLeaderboard;
}

void SteamLeaderboards::OnDownloaded(LeaderboardScoresDownloaded_t* result, bool io_failure)
{
	download_pending = false;
	if (io_failure)
	{
		std::cout << "Failed to download leaderboard" << std::endl;
		return;
	}
	auto predicate = [result](const Leaderboard& leaderboard) {
		return leaderboard.steam_leaderboard_handle == result->m_hSteamLeaderboard;
	};
	auto leaderboard_it = std::find_if(leaderboards_.begin(), leaderboards_.end(), predicate);
	if (leaderboard_it == leaderboards_.end())
	{
		assert(false);
		return;
	}

	leaderboard_it->second.downloaded_data = DownloadedLeaderboardData {};
	for (int index = 0; index < result->m_cEntryCount; index++)
	{
		LeaderboardEntry_t leaderboardEntry;
		int32 details[details_size];
		SteamUserStats()->GetDownloadedLeaderboardEntry(result->m_hSteamLeaderboardEntries, index, &leaderboardEntry, details, details_size);
		std::cout << "Leaderboard entry #" << index << std::endl;
		std::cout << "  score: " << leaderboardEntry.m_nScore << std::endl;
		std::cout << "  steam ID: " << leaderboardEntry.m_steamIDUser.ConvertToUint64() << std::endl;
		std::cout << "  global rank: " << leaderboardEntry.m_nGlobalRank << std::endl;
		std::cout << "  details: " << details[0] << std::endl;
	}
}
