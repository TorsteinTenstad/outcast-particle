#pragma once
#include "level.hpp"
#include "undo_system.hpp"
#include "utils/math.hpp"
#include <vector>

static sf::Vector2f LimitAndSnapToGrid(Level& level, sf::Vector2f position, float grid_size)
{
	sf::Vector2f level_size = level.GetSize();
	position.x = Clamp(position.x, 0.f, level_size.x);
	position.y = Clamp(position.y, 0.f, level_size.y);
	position.x -= std::fmod(position.x, grid_size);
	position.y -= std::fmod(position.y, grid_size);
	return position;
}

class MoveSelectedWithCursor : public MergeableUndoableAction<MoveSelectedWithCursor>
{
private:
	Level& level_;
	std::vector<int> entities_;
	std::vector<sf::Vector2f> original_positions_;
	std::vector<sf::Vector2f> new_positions_;

	friend class MoveSelectedWithCursor;

public:
	MoveSelectedWithCursor(Level& level, sf::Vector2f mouse_pos) :
		level_(level)
	{
		for (auto [entity, selected, position] : level.GetEntitiesWith<Selected, Position>())
		{
			assert(level.HasComponents<Editable>(entity));
			entities_.push_back(entity);
			original_positions_.push_back(position->position);
			new_positions_.push_back(LimitAndSnapToGrid(level, mouse_pos + selected->mouse_offset, BLOCK_SIZE / 2));
		}
	}

private:
	void SetPositions(std::vector<sf::Vector2f>& positions)
	{
		int i = 0;
		for (int entity : entities_)
		{
			sf::Vector2f& position = level_.GetComponent<Position>(entity)->position;
			position = positions[i++];
		}
	}

public:
	void Do() { SetPositions(new_positions_); }
	void Undo() { SetPositions(original_positions_); }

	bool TryMerge(const MoveSelectedWithCursor& next_action) override
	{
		if (next_action.entities_ != entities_)
		{
			return false;
		}
		new_positions_ = std::move(next_action.new_positions_);
		return true;
	}
};