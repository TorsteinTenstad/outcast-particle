#pragma once
#include "PCH.hpp"

class DrawInfo
{
public:
	std::string image_path;
	bool scale_to_fit = true;
	int quarter_turn_rotations = 0;
};

class DrawPriority
{
public:
	int draw_priority = 0;
};

class ChargeDependentDrawInfo
{};

class OrientationDependentDrawInfo
{};

class Background
{};

class VelocityDependentDrawLayer
{
public:
	int owned_entity = -1;
};