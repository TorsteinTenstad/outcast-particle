#include "utils/level_id.hpp"
#include "constants.hpp"
#include "utils/string_parsing.hpp"
#include <filesystem>

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

std::string GetLevelDisplayNameFromId(const std::string& level_id)
{
	const std::filesystem::path path { level_id };
	return SplitString(path.filename().stem().string(), "_").back();
}

bool IsMenu(const std::string& level_id)
{
	return level_id == MAIN_MENU || level_id == LEVEL_MENU || level_id == OPTIONS_MENU || level_id == KEY_CONFIG_MENU || level_id == GRAPHICS_AND_DISPLAY_MENU || level_id == MUSIC_AND_SOUND_MENU;
}