#pragma once
#include "cpr/cpr.h"
#include "records_manager.hpp"
#include <array>
#include <optional>
#include <string>

struct LeaderboardEntryDisplayInfo
{
	int rank;
	std::string steam_username;
	float time;
};

class ServerTransceiver : public RecordsListener
{
public:
	ServerTransceiver();
	void Update();
	void OnRecordUpdate(std::string level_id, int coins_collected, bool neutral_was_used, float time) override;
	std::vector<LeaderboardEntryDisplayInfo> GetLeaderboardDisplayInfo(std::string level_id, int coins_collected);

private:
	std::map<std::string, std::map<int, std::vector<LeaderboardEntryDisplayInfo>>> leaderboard_data_;

	void SendUsernameRequest(uint64_t user_id, std::string username);
	std::optional<cpr::AsyncResponse> pending_async_username_response_;
	void SendLeaderboardRequest(std::optional<uint64_t> user_id);
	std::optional<cpr::AsyncResponse> pending_async_leaderboard_response_;
	void SendScoreRequest(uint64_t user_id, std::string level_id, int coins_collected, bool neutral_was_used, float time);
	std::vector<cpr::AsyncResponse> pending_async_score_responses_;
};
