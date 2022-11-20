#pragma once

class Player
{
public:
	bool can_switch_charge = true;
	bool can_go_neutral = true;
	float move_force = 0;
	float default_charge = 0; // Initialized and used by PlayerSystem
};