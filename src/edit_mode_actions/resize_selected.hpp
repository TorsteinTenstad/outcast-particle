#pragma once
#include "level.hpp"
#include "undo_system.hpp"
#include <vector>

class ResizeSelected : public MergeableUndoableAction<ResizeSelected>
{
private:
	Level& level_;
	std::vector<Entity> entities_;
	std::vector<sf::Vector2f> original_sizes_;
	sf::Vector2f size_delta_;

	friend class ResizeSelected;

public:
	ResizeSelected(Level& level, sf::Vector2f size_delta) :
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
		for (Entity entity : entities_)
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
		for (Entity entity : entities_)
		{
			sf::Vector2f& width_and_height = level_.GetComponent<WidthAndHeight>(entity)->width_and_height;
			width_and_height = original_sizes_[i++];
		}
	}
	bool TryMerge(const ResizeSelected& next_action) override
	{
		if (next_action.entities_ != entities_)
		{
			return false;
		}
		size_delta_ += next_action.size_delta_;
		return true;
	}
};