#pragma once
#include <string>

std::string GetGroupNameFromId(const std::string& level_id);
std::string GetGroupDisplayNameFromGroupName(const std::string& group_name);
std::string GetGroupDisplayNameFromId(const std::string& level_id);
std::string GetLevelDisplayNameFromId(const std::string& level_id);
bool IsMenu(const std::string& level_id);