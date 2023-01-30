#include <fstream>
#include <string>
#include <vector>

#include "controls_config_serialization.hpp"
#include "level.hpp"
#include "string_parsing_utils.hpp"

template <typename Map>
void SaveMapToFile(std::string savefile_path, const Map& map)
{
	std::ofstream f(savefile_path);
	for (const auto& i : map)
	{
		std::string line;
		line += ToString(i.first);
		line += "; ";
		line += ToString(i.second);
		line += "\n";
		f << line;
	}
}

template <typename K, typename V>
void LoadMapFromFile(std::string savefile_path, std::map<K, V>& map)
{
	std::ifstream f(savefile_path);
	std::string line;
	K key;
	V value;
	while (getline(f, line))
	{
		std::vector<std::string> pair = SplitString(line, "; ");
		FromString(key, pair[0]);
		FromString(value, pair[1]);
		map[key] = value;
	}
}

//The MapOfMap-functions should only be called for maps with maps as values
template <typename K, typename V>
void SaveMapOfMapToFile(std::vector<std::string> savefile_paths, std::map<K, V> map)
{
	for (int i = 0; i < size(savefile_paths); ++i)
	{
		SaveMapToFile(savefile_paths[i], map[i]);
	}
}

template <typename K, typename V>
void LoadMapOfMapFromFile(std::vector<std::string> savefile_paths, std::map<K, V>& map)
{
	for (int i = 0; i < size(savefile_paths); ++i)
	{
		LoadMapFromFile(savefile_paths[i], map[i]);
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