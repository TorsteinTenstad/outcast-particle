#include "level_manager.hpp"
#include "folder_definitions.hpp"
#include "level.hpp"
#include "utils/level_id.hpp"
#include "utils/string_parsing.hpp"
#include <cassert>
#include <filesystem>
#include <fstream>

LevelManager::LevelManager(const std::filesystem::path& levels_dir) :
	levels_dir_(levels_dir)
{
	Load();
}

void LevelManager::Load()
{
	std::vector<std::string> corrupt_files;
	for (const auto& folder : std::filesystem::directory_iterator { levels_dir_ })
	{
		if (!folder.is_directory()) { continue; }
		std::string group = folder.path().stem().string();
		if (std::find(editable_level_groups.begin(), editable_level_groups.end(), GetGroupDisplayNameFromGroupName(group)) != editable_level_groups.end()) //Editable level groups should be visible even if empty
		{
			levels_[group];
		}

		for (const auto& level_file_path : std::filesystem::directory_iterator { folder.path() })
		{
			std::string level_id = level_file_path.path().string();
			assert(group == GetGroupNameFromId(level_id));
			levels_[group].push_back(level_id);
			Level level;
			Error err = level.LoadFromFile(level_id);
			if (err) { corrupt_files.push_back(level_id); }
		}
	}
	if (corrupt_files.size() > 0)
	{
		std::string message = "Failed to load game levels:";
		for (const std::string& filename : corrupt_files)
		{
			message += "\n\t" + filename;
		}
		globals.errors += Error(ErrorNumber::LOAD_LEVEL, message);
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

std::optional<std::string> LevelManager::ImportLevel(std::string path, std::string group_name)
{
	std::vector<std::string> filename_parts = SplitString(std::filesystem::path(path).stem().string(), "_");
	int level_number;
	Error err = FromString(level_number, filename_parts[0]);
	if (err)
	{
		level_number = levels_.at(group_name).size() > 0 ? GetLevelNumberFromId(levels_.at(group_name).back()) + 1 : 0;
	}
	std::string new_level_id = AssembleLevelId(group_name, level_number, filename_parts.back());
	Level level;
	err = level.LoadFromFile(path);
	if (err)
	{
		globals.errors += Error(ErrorNumber::LOAD_LEVEL, "Failed to import level\n" + path);
		return std::nullopt;
	}
	std::filesystem::copy_file(path, new_level_id);
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

std::string LevelManager::RenameLevel(const std::string& old_level_id, const std::string& new_display_name)
{
	std::string new_level_id = AssembleLevelId(GetGroupNameFromId(old_level_id), GetLevelNumberFromId(old_level_id), new_display_name);
	RawRenameLevel(old_level_id, new_level_id);
	return new_level_id;
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
