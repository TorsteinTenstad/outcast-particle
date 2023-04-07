
#include "blueprint.hpp"

std::string ToBlueprintTag(Blueprint blueprint)
{
    return std::vector<std::string>({
        "BPEditableEntity",
		"BPStaticParticle",
		"BPMovingParticle",
		"BPBlackHole",
		"BPPlayer",
		"BPLaser",
		"BPCoin",
		"BPWall",
		"BPBounceWall",
		"BPNoBounceWall",
		"BPGoal",
		"BPElectricField",
		"BPMagneticField",
		"BPTextPopupSpawner"})[(int(blueprint))];
}

Blueprint ToBlueprintEnum(std::string blueprint_tag)
{
    return std::map<std::string, Blueprint>({
        {"BPEditableEntity", BPEditableEntity},
		{"BPStaticParticle", BPStaticParticle},
		{"BPMovingParticle", BPMovingParticle},
		{"BPBlackHole", BPBlackHole},
		{"BPPlayer", BPPlayer},
		{"BPLaser", BPLaser},
		{"BPCoin", BPCoin},
		{"BPWall", BPWall},
		{"BPBounceWall", BPBounceWall},
		{"BPNoBounceWall", BPNoBounceWall},
		{"BPGoal", BPGoal},
		{"BPElectricField", BPElectricField},
		{"BPMagneticField", BPMagneticField},
		{"BPTextPopupSpawner", BPTextPopupSpawner}}).at(blueprint_tag);
}