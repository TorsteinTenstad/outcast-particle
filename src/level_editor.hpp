#pragma once
#include "level.hpp"
#include "undo_system.hpp"
#include <vector>

class LevelEditor : public UndoSystem
{
public:
	void SelectEntities(Level& level, std::vector<int> entity_ids);
	void DeselectAllEntities(Level& level);
};