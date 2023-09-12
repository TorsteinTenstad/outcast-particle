#include "utils/level_id.hpp"
#include "constants.hpp"
#include "folder_definitions.hpp"
#include "level_id.hpp"
#include "utils/string_parsing.hpp"
#include <cassert>
#include <filesystem>

// Level ID format: level\\<group-number>_<group-display-name>\\<level-number>_<level-display-name>.txt

int GetLevelNumberFromId(const std::string& level_id)
{
	int number;
	std::string number_str = SplitString(GetLevelNameFromId(level_id), "_").front();
	if (!IsNumeric(number_str))
	{
		assert(false);
		return 0;
	}
	FromString(number, number_str);
	return number;
}

std::string AssembleLevelId(std::string group_name, int level_number, std::string level_display_name)
{
	std::filesystem::path path { LEVELS_DIR };
	path.append(group_name);
	std::string level_name = ToString(level_number);
	if (level_name.length() == 1)
	{
		level_name = "0" + level_name;
	}
	level_name += "_" + level_display_name;
	path.append(level_name);
	path.replace_extension(".txt");
	return path.string();
}

std::string GetGroupNameFromId(const std::string& level_id)
{
	std::filesystem::path path { level_id };
	return (++path.remove_filename().begin())->string();
}

std::string GetGroupDisplayNameFromGroupName(const std::string& group_name)
{
	return SplitString(group_name, "_").back();
}

std::string GetGroupDisplayNameFromId(const std::string& level_id)
{
	return GetGroupDisplayNameFromGroupName(GetGroupNameFromId(level_id));
}

std::string GetLevelNameFromId(const std::string& level_id)
{
	const std::filesystem::path path { level_id };
	return path.filename().stem().string();
}

std::string GetLevelDisplayNameFromId(const std::string& level_id)
{
	return SplitString(GetLevelNameFromId(level_id), "_").back();
}

bool IsMenu(const std::string& level_id)
{
	return level_id == MAIN_MENU || level_id == LEVEL_MENU || level_id == OPTIONS_MENU || level_id == KEY_CONFIG_MENU || level_id == GRAPHICS_AND_DISPLAY_MENU || level_id == MUSIC_AND_SOUND_MENU || level_id == CREDITS_MENU;
}