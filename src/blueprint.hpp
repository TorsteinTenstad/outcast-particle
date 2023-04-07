#pragma once
#include <string>
#include <vector>
#include <map>

enum Blueprint
{
    BPEditableEntity = 0,
	BPStaticParticle = 1,
	BPMovingParticle = 2,
	BPBlackHole = 3,
	BPPlayer = 4,
	BPLaser = 5,
	BPCoin = 6,
	BPWall = 7,
	BPBounceWall = 8,
	BPNoBounceWall = 9,
	BPGoal = 10,
	BPElectricField = 11,
	BPMagneticField = 12,
	BPTextPopupSpawner = 13
};
    
std::string ToBlueprintTag(Blueprint blueprint);
Blueprint ToBlueprintEnum(std::string blueprint_tag);
