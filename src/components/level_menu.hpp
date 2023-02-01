#pragma once

class LevelMenuUI
{
public:
	bool initialized = false;

	std::string level_group;
	std::string at_level_id = "";
	std::string* level_image_identifier;
	std::string requested_level_group;

	int next_group_button_id;
	int prev_group_button_id;

	std::vector<int> button_entity_ids = {};
	std::vector<int> entity_ids = {};

	std::string* stats_string;
};