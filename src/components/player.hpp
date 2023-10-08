#pragma once

class Player
{
public:
	bool can_switch_charge = true;
	bool can_go_neutral = true;
	bool can_move_self = false;
	int move_force = 0;
};

class DeadPlayer
{
};

class PlayerBehaviors
{
public:
	float default_charge = 0;
	float is_neutral = false;
	bool switch_key_is_up = true;
};