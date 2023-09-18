#pragma once
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <vector>

class LevelManager
{
private:
	const std::filesystem::path& levels_dir_;
	std::map<std::string, std::vector<std::string>> levels_;
	void RawRenameLevel(const std::string& old_level_id, const std::string& new_level_id);

public:
	LevelManager(const std::filesystem::path& levels_dir);
	void Load();
	const std::map<std::string, std::vector<std::string>>& GetLevels() const;
	std::string CreateNewLevel(std::string group_name);
	std::optional<std::string> ImportLevel(std::string path, std::string group_name);
	bool DeleteLevel(std::string level_id);
	std::string RenameLevel(const std::string& old_level_id, const std::string& new_display_name);
};