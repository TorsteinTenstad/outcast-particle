#pragma once
#include <optional>

class StickyButton
{
public:
	bool enforce_down = false;
	std::optional<int> channel = std::nullopt;
};

class StickyButtonDown
{
};