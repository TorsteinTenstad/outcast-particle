
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
		"BPGoal",
		"BPElectricField",
		"BPMagneticField",
		"BPText",
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
		{"BPGoal", BPGoal},
		{"BPElectricField", BPElectricField},
		{"BPMagneticField", BPMagneticField},
		{"BPText", BPText},
		{"BPTextPopupSpawner", BPTextPopupSpawner}}).at(blueprint_tag);
}