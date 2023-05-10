#pragma once
#include <map>
#include <string>
#include <vector>

class LevelManager
{
private:
	std::map<std::string, std::vector<std::string>> levels_;
	void RawRenameLevel(const std::string& old_level_id, const std::string& new_level_id);

public:
	LevelManager(std::string levels_folder_path);
	const std::map<std::string, std::vector<std::string>>& GetLevels() const;
	std::string CreateNewLevel(std::string group_name);
	bool DeleteLevel(std::string level_id);
	std::string RenameLevel(const std::string& old_level_id, const std::string& new_display_name);
};