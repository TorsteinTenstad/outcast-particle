#pragma once
#include "level.hpp"
#include "undo_system.hpp"
#include <vector>

void Deselect(Level& level, std::vector<int> entity_ids);
void Select(Level& level, std::vector<int> entity_ids);
void LimitPosition(Level& level, sf::Vector2f& position, sf::Vector2f size);
void SnapToGrid(sf::Vector2f& v, float grid_size);
void LimitAndSnapPosition(Level& level, sf::Vector2f& position, sf::Vector2f size);

class LevelEditor : public UndoSystem
{
private:
	void FunctionalDo(std::function<void(void)>&& do_func, std::function<void(void)>&& undo_func);

public:
	void SelectEntities(Level& level, std::vector<int> entity_ids, bool deselect_others);
	void DeselectAllEntities(Level& level);
	void MoveSelectedEntities(Level& level, sf::Vector2f distance);
};