#pragma once
#include "level.hpp"

void OpenBlueprintMenu(Level& level);
std::optional<Entity> UpdateBlueprintMenu(Level& level);
void CloseBlueprintMenu(Level& level);
void ToggleBlueprintMenu(Level& level);