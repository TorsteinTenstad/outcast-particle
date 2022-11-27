#pragma once
#include "PCH.hpp"

class DrawInfo
{
public:
	std::string image_path;
	bool scale_to_fit = true;
	int draw_priority = 0;
	int quarter_turn_rotations = 0;
};

class ChargeDependentDrawInfo
{};

class OrientationDependentDrawInfo
{
public:
	std::string horisontal_image_path;
	std::string vertical_image_path;
};