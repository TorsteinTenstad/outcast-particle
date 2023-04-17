#include "components/animated_properties.hpp"
#include "components/draw_info.hpp"
#include "components/position.hpp"
#include "components/text.hpp"
#include "systems/_pure_DO_systems.hpp"

void AnimatedPropertiesSystem::Update(Level& level, float dt)
{
	for (auto [entity_id, animated_property, position] : level.GetEntitiesWith<AnimatedPosition, Position>())
	{
		position->position = animated_property->animation_func(globals.time - animated_property->start_time);
	}
	for (auto [entity_id, animated_property, fill_color] : level.GetEntitiesWith<AnimatedOpacity, FillColor>())
	{
		fill_color->color.a = animated_property->animation_func(globals.time - animated_property->start_time);
	}
	for (auto [entity_id, animated_property, text] : level.GetEntitiesWith<AnimatedOpacity, Text>())
	{
		text->color.a = animated_property->animation_func(globals.time - animated_property->start_time);
		text->outline_color.a = animated_property->animation_func(globals.time - animated_property->start_time);
	}
}
