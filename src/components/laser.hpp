#pragma once
#include <map>

class KillOnIntersection
{};

class Laser
{
public:
	bool kill_on_intersection_enabled = true;
};

class TogglingLaser
{
public:
	float period_s = 1;
	float duty_cycle = 1;
	float phase = 0;
};