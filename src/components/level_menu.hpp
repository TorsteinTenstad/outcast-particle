#pragma once

class LevelMenuUI
{
public:
	bool initialized = false;

	std::string level_image_identifier;
	int image_entity_iid = -1;

	int collected_coins = -1;
	int all_coins = -1;
	float best_completion_time = -1;
	float best_completion_time_all_coins = -1;
	int fewest_inputs = -1;
};

class LevelButton
{
public:
	int level;
};