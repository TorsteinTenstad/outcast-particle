#pragma once
#include "components/editable.hpp"
#include "components/physics.hpp"
#include "constants.hpp"
#include "cursor_and_keys.hpp"
#include "game_system.hpp"
#include "utils.hpp"

class EditModeSystem : public GameSystem
{
private:
	const float default_velocity_magnitude_change_sensitivity_ = 400;
	const float default_velocity_angle_change_sensitivity_ = PI / 2;
	const std::vector<Blueprint> blueprint_menu_entry_tags_ { BPStaticParticle, BPPlayer, BPLaser, BPWall, BPBounceWall, BPNoBounceWall, BPGoal, BPElectricField, BPMagneticField, BPCoin, BPBlackHole };

	bool blueprint_menu_is_open_ = false;

public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt);
	void OpenBlueprintMenu(Level& level);
	void CloseBlueprintMenu(Level& level);
};