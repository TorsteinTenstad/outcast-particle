#include "records_manager.hpp"
#include "utils/string_parsing.hpp"
#include <limits>

RecordsManager::RecordsManager(std::string savefile_path) :
	savefile_path_(savefile_path)
{
	FromFile(records_, savefile_path);
}

RecordsManager::~RecordsManager()
{
	ToFile(records_, savefile_path_);
}

void RecordsManager::UpdateRecord(std::string level_id, int coins_collected, bool neutral_was_used, float time)
{
	std::tuple<std::string, int, bool> key = { level_id, coins_collected, neutral_was_used };
	records_[key] = time;
}

std::optional<float> RecordsManager::GetRecord(std::string level_id, int coins_collected, bool neutral_was_used) const
{
	std::optional<float> record;
	for (const auto& [key, time] : records_)
	{
		auto [level_id_, i, neutral_was_used_] = key;
		if (neutral_was_used_ != neutral_was_used || coins_collected > i || level_id_ != level_id) { continue; }
		record = std::min(time, record.value_or(std::numeric_limits<float>::max()));
	}
	return record;
}

std::optional<float> RecordsManager::GetRecord(std::string level_id, int coins_collected) const
{
	std::optional<float> a = GetRecord(level_id, coins_collected, false);
	std::optional<float> b = GetRecord(level_id, coins_collected, true);
	if (!a.has_value() && !b.has_value()) { return std::nullopt; }
	return std::min(a.value_or(std::numeric_limits<float>::max()), b.value_or(std::numeric_limits<float>::max()));
}