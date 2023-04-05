#include "level_editor.hpp"
#include "utils/get_size.hpp"
#include "utils/math.hpp"

void Deselect(Level& level, std::vector<int> entity_ids)
{
	for (int id : entity_ids)
	{
		level.RemoveComponents<Selected>(id);
	}
}

void Select(Level& level, std::vector<int> entity_ids)
{
	for (int id : entity_ids)
	{
		level.EnsureExistenceOfComponent<Selected>(id);
	}
}

void LimitPosition(Level& level, sf::Vector2f& position, sf::Vector2f size)
{
	sf::Vector2f level_size = level.GetSize();
	position.x = Clamp(position.x, size.x / 2, level_size.x - size.x / 2);
	position.y = Clamp(position.y, size.y / 2, level_size.y - size.y / 2);
}

void SnapToGrid(sf::Vector2f& v, float grid_size)
{
	v.x -= std::fmod(v.x, grid_size);
	v.y -= std::fmod(v.y, grid_size);
}

void LimitAndSnapPosition(Level& level, sf::Vector2f& position, sf::Vector2f size)
{
	LimitPosition(level, position, size);
	SnapToGrid(position, BLOCK_SIZE / 2);
}

void LevelEditor::FunctionalDo(std::function<void(void)>&& do_func, std::function<void(void)>&& undo_func)
{
	Do(std::move(std::make_unique<FunctionalUndoableAction>(std::move(do_func), std::move(undo_func))));
}

void LevelEditor::SelectEntities(Level& level, std::vector<int> entity_ids, bool deselect_others)
{
	std::vector<int> entities_to_deselect = deselect_others ? level.GetIdsWithComponent<Selected>() : std::vector<int> {};
	FunctionalDo(
		[&level, entities_to_deselect, entity_ids]() { Deselect(level, entities_to_deselect); Select(level, entity_ids); },
		[&level, entities_to_deselect, entity_ids]() { Deselect(level, entity_ids); Select(level, entities_to_deselect); });
}

void LevelEditor::DeselectAllEntities(Level& level)
{
	std::vector<int> currently_selected = level.GetIdsWithComponent<Selected>();
	FunctionalDo(
		[&level]() { level.ClearComponent<Selected>(); },
		[&level, currently_selected = std::move(currently_selected)]() { Select(level, currently_selected); });
}

void LevelEditor::MoveSelectedEntities(Level& level, sf::Vector2f distance)
{
	std::vector<std::tuple<int, Selected*, Position*>> selected_entities_with_position = level.GetEntitiesWith<Selected, Position>();
	std::vector<std::tuple<int, sf::Vector2f>> ids_and_positions;

	for (auto [entity_id, selected, position] : selected_entities_with_position)
	{
		ids_and_positions.push_back({ entity_id, position->position });
	}

	FunctionalDo(
		[&level, selected_entities_with_position, distance]() {
			for (auto [entity_id, selected, position] : selected_entities_with_position)
			{
				sf::Vector2f size = GetSize(level, entity_id, false);
				position->position += distance;
				LimitAndSnapPosition(level, position->position, size);
			}
		},
		[&level, ids_and_positions]() {
			for (auto [entity_id, position] : ids_and_positions)
			{
				level.GetComponent<Position>(entity_id)->position = position;
			}
		}

	);
}
