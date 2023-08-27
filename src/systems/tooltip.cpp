#include "components/tooltip.hpp"
#include "_pure_DO_systems.hpp"
#include "components/button_events.hpp"
#include "components/draw_info.hpp"
#include "components/position.hpp"
#include "components/scale_with_level.hpp"
#include "components/size.hpp"
#include "entity_creation.hpp"
#include "utils/get_size.hpp"

constexpr float TOOLTIP_BACKGROUND_PADDING = 20;

void TooltipSystem::Update(Level& level, float dt)
{
	if (!globals.general_config.show_show_tooltips) { return; }
	sf::Vector2f level_size = level.GetSize();
	for (const auto [entity, tooltip, draw_priority, position] : level.GetEntitiesWith<Tooltip, DrawPriority, Position>())
	{
		if (!level.HasComponents<Hovered>(entity))
		{
			DeleteChildrenOwnedBy<Tooltip>(level, entity);
			tooltip->hovered_s = 0;
			continue;
		}
		tooltip->hovered_s += dt;
		if (tooltip->hovered_s < tooltip->delay_s) { continue; }
		std::function<Entity(Level&)> create_popup = [content = tooltip->text,
														 draw_priority = draw_priority->draw_priority + 2](Level& level) {
			Entity entity = level.CreateEntity();
			level.AddComponents<WidthAndHeight, Position, DrawInfo, ScaleWithLevel>(entity);
			level.AddComponent<DrawPriority>(entity)->draw_priority = draw_priority;
			level.AddComponent<Shader>(entity)->fragment_shader_path = "shaders\\tooltip_background.frag";
			Text* text = level.AddComponent<Text>(entity);
			text->content = content;
			text->size = 100 * level.GetScale();
			text->origin = TextOrigin::CENTER_CENTER;
			text->render = false;
			text->color = sf::Color(220, 220, 220);
			return entity;
		};
		Entity popup = GetSingletonChildId<Tooltip>(level, entity, create_popup);

		Text* text = level.GetComponent<Text>(popup);
		if (text->render) { continue; }
		if (!text->result_size.has_value()) { continue; }
		sf::Vector2f width_and_height = text->result_size.value() + sf::Vector2f(1, 1) * 2.f * TOOLTIP_BACKGROUND_PADDING;
		sf::Vector2f entity_size = GetSize(level, entity, false);
		sf::Vector2f pos = position->position + entity_size / 2.f + width_and_height / 2.f;
		if (pos.x + width_and_height.x / 2 > level_size.x)
		{
			pos.x -= entity_size.x + width_and_height.x;
		}
		if (pos.y + width_and_height.y / 2 > level_size.y)
		{
			pos.y -= entity_size.y + width_and_height.y;
		}
		level.GetComponent<Position>(popup)->position = pos;
		level.GetComponent<WidthAndHeight>(popup)->width_and_height = width_and_height;
		text->render = true;
	}
}