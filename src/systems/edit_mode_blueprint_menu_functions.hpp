#pragma once
#include "level.hpp"

void OpenBlueprintMenu(Level& level);
std::optional<Entity> UpdateBlueprintMenu(Level& level);
void CloseBlueprintMenu(Level& level);
void ToggleBlueprintMenu(Level& level);

void OpenMusicMenu(Level& level);
void CloseMusicMenu(Level& level);
void ToggleMusicMenu(Level& level);