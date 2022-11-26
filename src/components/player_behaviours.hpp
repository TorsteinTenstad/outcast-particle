#pragma once
#include "PCH.hpp"

class PlayerBehaviours
{
public:
	float default_charge = 0;
	float default_radius = 0;
	float radius_modifier = 1;
	std::map<float, float> radius_animation;
};