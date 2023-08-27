#include "components/scale_with_level.hpp"
#include "_pure_DO_systems.hpp"
#include "components/position.hpp"
#include "components/size.hpp"
#include "components/text.hpp"

void ScaleWithLevelSystem::Update(Level& level, float dt)
{
	float level_scale = level.GetScale();
	for (auto& [entity, scale_with_level, width_and_height, position] : level.GetEntitiesWith<ScaleWithLevel, WidthAndHeight, Position>())
	{
		if (!scale_with_level->level_scale.has_value()) { scale_with_level->level_scale = level_scale; }
		float scale_rate = level_scale / scale_with_level->level_scale.value();
		scale_with_level->level_scale = level_scale;
		position->position *= scale_rate;
		width_and_height->width_and_height *= scale_rate;
		if (Text* text = level.RawGetComponent<Text>(entity))
		{
			text->size *= scale_rate;
		}
	}
}