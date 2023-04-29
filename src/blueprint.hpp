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
	BPGoal = 8,
	BPElectricField = 9,
	BPMagneticField = 10,
	BPTextPopupSpawner = 11
};
    
std::string ToBlueprintTag(Blueprint blueprint);
Blueprint ToBlueprintEnum(std::string blueprint_tag);
