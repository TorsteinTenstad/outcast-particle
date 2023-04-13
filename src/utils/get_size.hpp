#pragma once
#include "level.hpp"

static sf::Vector2f GetSize(Level& level, int entity_id, bool allow_no_size)
{
	if (WidthAndHeight* width_and_height = level.RawGetComponent<WidthAndHeight>(entity_id))
	{
		return width_and_height->width_and_height;
	}
	if (Radius* radius = level.RawGetComponent<Radius>(entity_id))
	{
		return sf::Vector2f(1, 1) * 2.f * radius->radius;
	}
	assert(allow_no_size);
	return sf::Vector2f(0, 0);
}