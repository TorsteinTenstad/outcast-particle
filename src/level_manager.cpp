#include "level_manager.hpp"
#include "folder_definitions.hpp"
#include "utils/level_id.hpp"
#include <cassert>
#include <filesystem>
#include <fstream>

LevelManager::LevelManager(std::string levels_folder)
{
	const std::filesystem::path levels_folder_path { levels_folder };
	for (const auto& folder : std::filesystem::directory_iterator { levels_folder_path })
	{
		if (!folder.is_directory())
		{
			continue;
		}
		for (const auto& level_file_path : std::filesystem::directory_iterator { folder.path() })
		{
			std::string level_id = level_file_path.path().string();
			std::string group = GetGroupNameFromId(level_id);
			levels_[group].push_back(level_id);
		}
	}
}

const std::map<std::string, std::vector<std::string>>& LevelManager::GetLevels() const
{
	return levels_;
}

std::string LevelManager::CreateNewLevel(std::string group_name)
{
	int level_number = levels_.at(group_name).size() > 0 ? GetLevelNumberFromId(levels_.at(group_name).back()) + 1 : 0;
	std::string level_display_name = "Unnamed level";
	std::string new_level_id = AssembleLevelId(group_name, level_number, level_display_name);
	std::filesystem::copy_file(NEW_LEVEL_TEMPLATE_FILE, new_level_id);
	levels_.at(group_name).push_back(new_level_id);
	return new_level_id;
}

bool LevelManager::DeleteLevel(std::string level_id)
{
	std::string group_name = GetGroupNameFromId(level_id);
	auto group_it = levels_.find(group_name);
	if (group_it == levels_.end())
	{
		assert(false);
		return false;
	}
	group_it->second.erase(
		std::remove(
			group_it->second.begin(),
			group_it->second.end(),
			level_id),
		group_it->second.end());

	return std::filesystem::remove(level_id);
}

void LevelManager::RenameLevel(const std::string& old_level_id, const std::string& new_display_name)
{
	std::string new_level_id = AssembleLevelId(GetGroupNameFromId(old_level_id), GetLevelNumberFromId(old_level_id), new_display_name);
	return RawRenameLevel(old_level_id, new_level_id);
}

void LevelManager::RawRenameLevel(const std::string& old_level_id, const std::string& new_level_id)
{
	std::string group_name = GetGroupNameFromId(old_level_id);
	auto group_it = levels_.find(group_name);
	if (group_it == levels_.end())
	{
		assert(false);
		return;
	}
	auto level_it = std::find(
		group_it->second.begin(),
		group_it->second.end(),
		old_level_id);

	if (level_it == group_it->second.end())
	{
		assert(false);
		return;
	}
	std::filesystem::rename(old_level_id, new_level_id);
	*level_it = new_level_id;
}