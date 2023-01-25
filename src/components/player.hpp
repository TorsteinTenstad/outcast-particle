#pragma once

class Player
{
public:
	bool can_switch_charge = true;
	bool can_go_neutral = true;
	float move_force = 0;
};

class PlayerBehaviors
{
public:
	float default_charge = 0;
	float is_neutral = false;
};