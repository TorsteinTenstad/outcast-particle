#pragma once
#include <string>
#include <vector>

class LevelMenuUI
{
public:
	bool initialized = false;

	inline static std::string last_at_level_id;
	std::string at_level_id = "";
	std::string* level_image_identifier;

	int next_group_button_id;
	int prev_group_button_id;
	int dot_indicator_id;

	std::vector<int> button_entity_ids = {};

	std::vector<int> stats_block_ids = {};
};