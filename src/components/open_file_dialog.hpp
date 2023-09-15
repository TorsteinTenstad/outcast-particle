#pragma once
#include <optional>
#include <string>

class OpenFileDialog
{
public:
	bool show = true;
	std::optional<std::string> path;
};