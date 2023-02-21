#include <fstream>
#include <string>
#include <vector>

#include "controls_config_serialization.hpp"
#include "level.hpp"
#include "string_parsing_utils.hpp"

template <typename K, typename V>
inline std::string ToString(const std::map<K, V>& map)
{
	std::string str_rep;
	for (const auto& i : map)
	{
		str_rep += ToString(i.first);
		str_rep += "; ";
		str_rep += ToString(i.second);
		str_rep += "\n";
	}
	return str_rep;
}

template <typename K, typename V>
inline void FromString(std::map<K, V>& map, std::string s)
{
	std::vector<std::string> keys_and_values = SplitString(s, "\n");
	K key;
	V value;
	for (auto& i : keys_and_values)
	{
		std::vector<std::string> pair = SplitString(i, "; ");
		FromString(key, pair[0]);
		FromString(value, pair[1]);
		map[key] = value;
	}
}

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
		}
		else
		{
			FromString(value, line);
			map_of_map[key] = value;
		}
	}
}

void SaveOptionsToFile(std::string savefile_path)
{
	std::string key_string;
	std::ofstream f(savefile_path);
	SerializeComponent(globals.key_config, key_string);
	f << key_string;
}

void LoadOptionsFromFile(std::string savefile_path)
{
	std::ifstream f(savefile_path);
	std::string key_string;
	f >> key_string;
	DeserializeComponent(globals.key_config, key_string);
}
