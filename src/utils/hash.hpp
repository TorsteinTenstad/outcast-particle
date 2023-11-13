#include <fstream>
#include <iostream>
#include <sstream>

std::uint64_t HashFileContents(const std::string& filePath)
{
	std::ifstream file(filePath, std::ios::binary);

	if (!file.is_open()) { return 0; }

	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string content = buffer.str();

	std::uint64_t hash = 0;
	for (char c : content)
	{
		hash ^= static_cast<std::uint64_t>(c) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	}

	return hash;
}