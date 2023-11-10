#pragma once
#include <string>

class ServerTransceiver
{
private:
public:
	ServerTransceiver();
	void UpdateLeaderboardRecord(std::string level_id, int coins_collected, bool neutral_was_used, float time);
};
