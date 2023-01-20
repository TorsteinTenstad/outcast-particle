#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "level.hpp"
#include "string_parsing_utils.hpp"
#include "systems/coin.hpp"

std::string LevelAchievementString(Level level, std::string savefile_path, int coin_record, float completion_time_record)
{
	std::string str_rep = "\"";
	str_rep += savefile_path;
	str_rep += "\" Coin Record; ";
	str_rep += ToString(coin_record);
	str_rep += " Completion Time Record; ";
	str_rep += ToString(completion_time_record);
	return str_rep;
}

void Level::SaveRecordToFile(std::string savefile_path, int coin_record, float completion_time_record)
{

	std::ifstream f_in("user\\level_stats.txt");
	std::ofstream f_out("user\\level_stats_temp.txt");
	std::string line;
	bool level_found = false;
	while (std::getline(f_in, line))
	{
		std::string level_tag = GetSubstrBetween(line, "\"", "\"");
		if (savefile_path == level_tag)
		{
			std::string str_rep = LevelAchievementString(*this, savefile_path, coin_record, completion_time_record);
			line = str_rep;
			level_found = true;
		}
		line += "\n";
		f_out << line;
	}
	if (!level_found)
	{
		std::string line = LevelAchievementString(*this, savefile_path, coin_record, completion_time_record);
		f_out << line;
	}
	f_in.close();
	f_out.close();
	remove("user\\level_stats.txt");
	rename("user\\level_stats_temp.txt", "user\\level_stats.txt");
}
