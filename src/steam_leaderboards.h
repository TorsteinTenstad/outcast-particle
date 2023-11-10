#pragma once
#include "records_manager.hpp"
#include "steam/steam_api.h"

constexpr int details_size = 1;

struct DownloadedLeaderboardData
{
	int score;
	int rank;
};

struct Leaderboard
{
	std::optional<SteamLeaderboard_t> steam_leaderboard_handle;
	std::optional<DownloadedLeaderboardData> downloaded_data;
};

class SteamLeaderboards
{
public:
	SteamLeaderboards(const RecordsManager& records_manager);

	void Update();

private:
	std::map<std::string, Leaderboard> leaderboards_;

	CCallResult<SteamLeaderboards, LeaderboardFindResult_t> find_leaderboard_call_result_;
	bool find_leaderboard_pending = false;
	void OnLeaderboardFound(LeaderboardFindResult_t* result, bool io_failure);

	CCallResult<SteamLeaderboards, LeaderboardScoresDownloaded_t> download_call_result_;
	bool download_pending = false;
	void OnDownloaded(LeaderboardScoresDownloaded_t* result, bool io_failure);
};