#pragma once
#include <filesystem>
#include <map>
#include <optional>
#include <string>

class RecordsManager
{
private:
	std::filesystem::path savefile_path_;

public:
	std::map<std::tuple<std::string, int, bool>, float> records_;
	RecordsManager(const std::filesystem::path& savefile);
	~RecordsManager();

	void UpdateRecord(std::string level_id, int coins_collected, bool neutral_was_used, float time);
	std::optional<float> GetRecord(std::string level_id, int coins_collected, bool neutral_was_used) const;
	std::optional<float> GetRecord(std::string level_id, int coins_collected) const;
	std::optional<float> GetRecord(std::string level_id) const;
	int GetCoinsCollected(std::string level_id) const;
};