#pragma once
#include <map>
#include <optional>
#include <string>
#include <vector>

class ReDrawLevelMenuRequest
{
public:
	std::optional<std::string> at_group;
	std::optional<std::string> at_level_id;
};

class LevelMenuUI
{
public:
	inline static std::map<std::string, std::string> last_at_level_id;
	std::optional<std::string> at_group;
	std::optional<std::string> at_level_id;

	int level_preview_id;
	int next_group_button_id;
	int prev_group_button_id;
	int dot_indicator_id;

	std::optional<int> new_level_button;

	std::vector<int> button_entity_ids = {};
	std::vector<int> edit_name_button_entity_ids = {};
	std::vector<int> delete_level_button_entity_ids = {};

	std::vector<int> stats_block_ids = {};
};