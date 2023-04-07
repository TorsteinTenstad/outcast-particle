#pragma once
#include "level.hpp"
#include "undo_system.hpp"
#include <vector>

void Deselect(Level& level, std::vector<int> entity_ids);
void Select(Level& level, std::vector<int> entity_ids);
void LimitPosition(Level& level, sf::Vector2f& position, sf::Vector2f size);
void SnapToGrid(sf::Vector2f& v, float grid_size);
void LimitAndSnapPosition(Level& level, sf::Vector2f& position, sf::Vector2f size);

class ResizeEntities : public UndoableAction
{
private:
	Level& level_;
	std::vector<int> entities_;
	std::vector<sf::Vector2f> original_sizes_;
	sf::Vector2f size_delta_;

	friend class ResizeEntities;

public:
	ResizeEntities(Level& level, sf::Vector2f size_delta) :
		level_(level),
		size_delta_(size_delta)
	{
		for (auto [entity, selected, width_and_height] : level.GetEntitiesWith<Selected, WidthAndHeight>())
		{
			assert(level.HasComponents<Editable>(entity));
			entities_.push_back(entity);
			original_sizes_.push_back(width_and_height->width_and_height);
		}
	}
	void Do()
	{
		for (int entity : entities_)
		{
			sf::Vector2f& width_and_height = level_.GetComponent<WidthAndHeight>(entity)->width_and_height;
			float smallest_allowed_size = level_.GetComponent<Editable>(entity)->smallest_allowed_size;
			width_and_height += smallest_allowed_size * size_delta_;
			width_and_height.x = std::max(width_and_height.x, smallest_allowed_size);
			width_and_height.y = std::max(width_and_height.y, smallest_allowed_size);
		}
	}
	void Undo()
	{
		int i = 0;
		for (int entity : entities_)
		{
			sf::Vector2f& width_and_height = level_.GetComponent<WidthAndHeight>(entity)->width_and_height;
			width_and_height = original_sizes_[i++];
		}
	}
	std::optional<std::unique_ptr<UndoableAction>> TryMerge(std::unique_ptr<UndoableAction> other) override
	{
		if (typeid(*this) != typeid(*other))
		{
			return other;
		}
		auto other_modify_entity_size = static_cast<ResizeEntities*>(other.get());
		if (other_modify_entity_size->entities_ != entities_)
		{
			return other;
		}
		size_delta_ += other_modify_entity_size->size_delta_;
		return {};
	}
};

class LevelEditor : public UndoSystem
{
private:
	void FunctionalDo(std::function<void(void)>&& do_func, std::function<void(void)>&& undo_func);

public:
	void SelectEntities(Level& level, std::vector<int> entity_ids, bool deselect_others);
	void DeselectAllEntities(Level& level);
	void MoveSelectedEntities(Level& level, sf::Vector2f distance);
};