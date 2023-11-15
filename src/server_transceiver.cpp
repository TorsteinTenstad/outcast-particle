#include "server_transceiver.hpp"
#include "cpr/cpr.h"
#include "globals.hpp"
#include "json.hpp"
#include "utils/container_operations.hpp"
#include "utils/hash.hpp"
#include "utils/level_id.hpp"
#include <chrono>
#include <iostream>
#include <string>

#define SERVER_URL "http://volatile-particle.deno.dev/"

int TimeToServerFormat(float time) { return static_cast<int>(time * 1000.f); }
float TimeFromServerFormat(int server_format) { return static_cast<float>(server_format) / 1000.f; }

ServerTransceiver::ServerTransceiver()
{
	if (globals.steam_user_id.has_value() && globals.steam_username.has_value())
	{
		SendUsernameRequest(globals.steam_user_id.value(), globals.steam_username.value());
	}
	else
	{
		SendLeaderboardRequest(std::nullopt); // Get a non-personalized leaderboard
	}
}

void ServerTransceiver::Update()
{
	if (pending_async_username_response_.has_value())
	{
		auto& async_response = pending_async_username_response_.value();
		if (async_response.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
		{
			auto response = async_response.get();
			if (response.status_code != 200)
			{
				std::cout << "Error " << response.status_code << ": " << response.text << std::endl;
			}
			pending_async_username_response_.reset();
			SendLeaderboardRequest(globals.steam_user_id);
		}
	}
	if (pending_async_leaderboard_response_.has_value())
	{
		auto& async_response = pending_async_leaderboard_response_.value();
		if (!pending_async_score_responses_.empty())
		{
			auto cancellation_result = async_response.Cancel();

			pending_async_leaderboard_response_.reset();
		}
		else
		{
			if (async_response.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
			{
				auto response = async_response.get();
				if (response.status_code != 200)
				{
					std::cout << "Error " << response.status_code << ": " << response.text << std::endl;
				}
				else
				{
					has_received_leaderboard = true;
					leaderboard_data_.clear();
					nlohmann::json json_response = nlohmann::json::parse(response.text);
					for (auto const& [level_display_name, level_leaderboard] : json_response.items())
					{
						for (auto const& [coins, leaderboard] : level_leaderboard.items())
						{
							for (auto const& entry : leaderboard)
							{
								leaderboard_data_[level_display_name][std::stoi(coins)].push_back(
									LeaderboardEntryDisplayInfo {
										entry["rank"],
										entry["username"],
										TimeFromServerFormat(entry["score"]),
									});
							}
						}
					}
				}
				pending_async_leaderboard_response_.reset();
			}
		}
	}
	if (!pending_async_score_responses_.empty())
	{
		for (auto it = pending_async_score_responses_.begin(); it != pending_async_score_responses_.end();)
		{
			auto& async_response = *it;
			if (async_response.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
			{
				auto response = async_response.get();
				if (response.status_code != 200)
				{
					std::cout << "Error " << response.status_code << ": " << response.text << std::endl;
				}
				it = pending_async_score_responses_.erase(it);
			}
			else
			{
				++it;
			}
		}
		if (pending_async_score_responses_.empty())
		{
			SendLeaderboardRequest(globals.steam_user_id);
		}
	}
}

void ServerTransceiver::OnRecordUpdate(std::string level_id, int coins_collected, bool neutral_was_used, float time)
{
	if (globals.steam_user_id.has_value())
	{
		SendScoreRequest(globals.steam_user_id.value(), level_id, coins_collected, neutral_was_used, time);
	}
}

inline std::string GetServerLevelId(std::string level_id)
{
	std::string server_level_id = GetLevelDisplayNameFromId(level_id);
	if (Contains(editable_level_groups, GetGroupDisplayNameFromId(level_id)))
	{
		server_level_id += std::string("_") + std::to_string(HashFileContents(level_id));
	}
	return server_level_id;
}

std::vector<LeaderboardEntryDisplayInfo> ServerTransceiver::GetLeaderboardDisplayInfo(std::string level_id, int coins_collected) const
{
	auto level_leaderboard_it = leaderboard_data_.find(GetServerLevelId(level_id));
	if (level_leaderboard_it == leaderboard_data_.end()) { return {}; }
	auto leaderboard_it = level_leaderboard_it->second.find(coins_collected);
	if (leaderboard_it == level_leaderboard_it->second.end()) { return {}; }
	return leaderboard_it->second;
}

void ServerTransceiver::SendUsernameRequest(uint64_t user_id, std::string username)
{
	assert(!pending_async_username_response_.has_value());
	pending_async_username_response_ = cpr::PostAsync(
		cpr::Url { std::string(SERVER_URL) + std::string("/username") },
		cpr::Body { nlohmann::json { { "userId", user_id }, { "username", username } }.dump() },
		cpr::Header { { "Content-Type", "application/json" } });
}
void ServerTransceiver::SendLeaderboardRequest(std::optional<uint64_t> user_id)
{
	assert(!pending_async_leaderboard_response_.has_value());
	pending_async_leaderboard_response_ = cpr::GetAsync(
		cpr::Url { std::string(SERVER_URL) + std::string("/leaderboards") },
		cpr::Parameters { { "userId", std::to_string(user_id.value_or(0)) } });
}
void ServerTransceiver::SendScoreRequest(uint64_t user_id, std::string level_id, int coins_collected, bool neutral_was_used, float time)
{
	pending_async_score_responses_.push_back(
		cpr::PostAsync(
			cpr::Url { std::string(SERVER_URL) + std::string("/score") },
			cpr::Body {
				nlohmann::json {
					{ "userId", user_id },
					{ "level", GetServerLevelId(level_id) },
					{ "coins", coins_collected },
					{ "neutralWasUsed", neutral_was_used },
					{ "score", TimeToServerFormat(time) },
				}
					.dump() },
			cpr::Header { { "Content-Type", "application/json" } }));
}