#pragma once
#include "PCH.hpp"

class DrawInfo
{
public:
	std::string image_path;
	bool scale_to_fit = false;
	int draw_priority = 0;
	int quarter_turn_rotations = 0;
};

class ChargeDependentDrawInfo
{
public:
	std::string positive_charge_image_path;
	std::string neutral_charge_image_path;
	std::string negative_charge_image_path;
};

class OrientationDependentDrawInfo
{
public:
	std::string horisontal_image_path;
	std::string vertical_image_path;
};