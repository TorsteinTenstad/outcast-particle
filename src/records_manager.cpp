#include "records_manager.hpp"
#include "globals.hpp"
#include "utils/string_manip.hpp"
#include "utils/string_parsing.hpp"
#include <limits>

RecordsManager::RecordsManager(const std::filesystem::path& savefile) :
	savefile_path_(savefile)
{
	Error err = FromFile(records_, savefile);
	if (err)
	{
		std::filesystem::path backup_path = savefile.parent_path() / (savefile.stem().string() + "_corrupt_backup_" + GetDateTimeIdentifier() + savefile.extension().string());
		try
		{
			std::filesystem::rename(savefile, backup_path);
			globals.errors += Error(ErrorNumber::LOAD_RECORDS, "Previously stored records was corrupted, and is backed up at\n" + backup_path.string());
		}
		catch (const std::filesystem::filesystem_error& e)
		{
		}
	}
}

RecordsManager::~RecordsManager()
{
	ToFile(records_, savefile_path_);
}

void RecordsManager::UpdateRecord(std::string level_id, int coins_collected, bool neutral_was_used, float time)
{
	std::tuple<std::string, int, bool> key = { level_id, coins_collected, neutral_was_used };
	auto existing_entry = records_.find(key);
	if (existing_entry != records_.end() && existing_entry->second <= time) { return; }
	records_[key] = time;
	for (const auto& listener : listeners_)
	{
		listener->OnRecordUpdate(level_id, coins_collected, neutral_was_used, time);
	}
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

std::optional<float> RecordsManager::GetRecord(std::string level_id) const
{
	std::optional<float> record;
	for (const auto& [key, time] : records_)
	{
		auto [level_id_, i, neutral_was_used_] = key;
		if (level_id_ != level_id) { continue; }
		record = std::min(time, record.value_or(std::numeric_limits<float>::max()));
	}
	return record;
}

int RecordsManager::GetCoinsCollected(std::string level_id) const
{
	int max_coins_collected = 0;
	for (const auto& [key, time] : records_)
	{
		auto [level_id_, i, neutral_was_used_] = key;
		if (level_id_ != level_id) { continue; }
		max_coins_collected = std::max(max_coins_collected, i);
	}
	return max_coins_collected;
}