#pragma once
#include <string>

enum class PreferredTooltipCorner
{
	TOP_RIGHT,
	TOP_LEFT,
	BOTTOM_RIGHT,
	BOTTOM_LEFT,
};

class Tooltip
{
public:
	float delay_s = 0.8;
	float hovered_s = 0;
	std::string text;
	PreferredTooltipCorner preferred_corner = PreferredTooltipCorner::BOTTOM_RIGHT;
};