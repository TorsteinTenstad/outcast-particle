#include "level_editor.hpp"

static std::function<void(void)> CreateSelectEntitiesFunc(Level& level, std::vector<int> entity_ids)
{
	return [&level, entity_ids]() {
		for (int id : entity_ids)
		{
			level.EnsureExistenceOfComponent<Selected>(id);
		}
	};
}

static std::function<void(void)> DeselectEntities(Level& level, std::vector<int> entity_ids)
{
	return [&level, entity_ids]() {
		for (int id : entity_ids)
		{
			level.RemoveComponents<Selected>(id);
		}
	};
}

void LevelEditor::SelectEntities(Level& level, std::vector<int> entity_ids)
{
	Do(
		CreateSelectEntitiesFunc(level, entity_ids),
		DeselectEntities(level, entity_ids));
}

void LevelEditor::DeselectAllEntities(Level& level)
{
	std::vector<int> entity_ids = level.GetIdsWithComponent<Selected>();
	Do(
		[&level]() { level.ClearComponent<Selected>(); },
		CreateSelectEntitiesFunc(level, entity_ids));
}
