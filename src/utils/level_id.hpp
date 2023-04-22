#pragma once
#include <string>

int GetLevelNumberFromId(const std::string& level_id);
std::string AssembleLevelId(std::string group_name, int level_number, std::string level_display_name);
std::string GetGroupNameFromId(const std::string& level_id);
std::string GetGroupDisplayNameFromGroupName(const std::string& group_name);
std::string GetGroupDisplayNameFromId(const std::string& level_id);
std::string GetLevelNameFromId(const std::string& level_id);
std::string GetLevelDisplayNameFromId(const std::string& level_id);
bool IsMenu(const std::string& level_id);