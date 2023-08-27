#include "components/tooltip.hpp"
#include "_pure_DO_systems.hpp"
#include "components/button_events.hpp"
#include "components/draw_info.hpp"
#include "components/position.hpp"
#include "components/size.hpp"
#include "entity_creation.hpp"
#include "utils/get_size.hpp"

void TooltipSystem::Update(Level& level, float dt)
{
	for (const auto [entity, tooltip, draw_priority, position] : level.GetEntitiesWith<Tooltip, DrawPriority, Position>())
	{
		if (!level.HasComponents<Hovered>(entity))
		{
			DeleteChildrenOwnedBy<Tooltip>(level, entity);
			continue;
		}
		std::function<Entity(Level&)> create_popup = [position = position->position + GetSize(level, entity, false) / 2.f,
														 content = tooltip->text,
														 draw_priority = draw_priority->draw_priority + 1](Level& level) {
			Entity entity = level.CreateEntity();
			level.AddComponent<Position>(entity)->position = position;
			level.AddComponent<DrawPriority>(entity)->draw_priority = draw_priority;
			level.AddComponent<Shader>(entity)->fragment_shader_path = "shaders\\scroll.frag";
			Text* text = level.AddComponent<Text>(entity);
			text->content = content;
			text->size = 60;
			text->origin = TextOrigin::TOP_LEFT;
			level.AddComponent<WidthAndHeight>(entity);
			return entity;
		};
		Entity popup = GetSingletonChildId<Tooltip>(level, entity, create_popup);
	}
}