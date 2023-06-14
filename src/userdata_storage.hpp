#include <fstream>
#include <string>
#include <vector>

#include "controls_config_serialization.hpp"
#include "general_user_config_serialization.hpp"
#include "level.hpp"
#include "utils/string_parsing.hpp"

//The MapOfMap-functions should only be called for maps with maps as values
template <typename K, typename V>
void SaveMapOfMapToFile(std::string savefile_path, std::map<K, V> map_of_map)
{
	std::string map_of_map_string;
	for (auto& [key, value] : map_of_map)
	{
		map_of_map_string += "Key: " + ToString(key) + " \n";
		map_of_map_string += ToString(value);
	}
	std::ofstream f(savefile_path);
	f << map_of_map_string;
}

template <typename K, typename V>
void LoadMapOfMapFromFile(std::string savefile_path, std::map<K, V>& map_of_map)
{
	std::ifstream f(savefile_path);
	std::string line;
	K key;
	V value;
	while (std::getline(f, line))
	{
		if (line.find("Key: ") == 0)
		{
			FromString(key, line.substr(5));
			value.clear();
		}
		else
		{
			FromString(value, line);
			map_of_map[key] = value;
		}
	}
}

void SaveOptionsToFile(std::string savefile_path_key_config, std::string savefile_path_general_config)
{
	std::string key_string;
	std::ofstream key_f(savefile_path_key_config);
	SerializeComponent(&globals.key_config, key_string);
	key_f << key_string;

	std::string general_string;
	std::ofstream general_f(savefile_path_general_config);
	SerializeComponent(&globals.general_config, general_string);
	general_f << general_string;
}

void LoadOptionsFromFile(std::string savefile_path_key_config, std::string savefile_path_general_config)
{
	std::ifstream key_f(savefile_path_key_config);
	if (key_f.is_open())
	{
		std::string key_string;
		key_f >> key_string;
		DeserializeComponent(&globals.key_config, key_string);
	}

	std::ifstream general_f(savefile_path_general_config);
	if (general_f.is_open())
	{
		std::string general_string;
		general_f >> general_string;
		DeserializeComponent(&globals.general_config, general_string);
	}
}
