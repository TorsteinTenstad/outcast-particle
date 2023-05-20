#include "components/text_popup.hpp"
#include "components/animated_properties.hpp"
#include "components/draw_info.hpp"
#include "components/intersection.hpp"
#include "components/player.hpp"
#include "components/position.hpp"
#include "components/scheduled_delete.hpp"
#include "components/size.hpp"
#include "components/text.hpp"
#include "entity_creation.hpp"
#include "level.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "utils/math.hpp"

class TextPopupEntityDeleteFlag
{};

void TextPopupSystem::Update(Level& level, float dt)
{
	for (auto [entity, text_popup_spawner, draw_info] : level.GetEntitiesWith<TextPopupSpawner, DrawInfo>())
	{
		draw_info->image_path = level.GetMode() == EDIT_MODE ? "content\\textures\\white.png" : "content\\textures\\transparent.png";
	}
	if (level.GetMode() != PLAY_MODE || level.ComputeState() != PLAYING)
	{
		level.DeleteEntitiesWith<TextPopupEntityDeleteFlag>();
		return;
	}
	for (auto [entity, player, intersection] : level.GetEntitiesWith<Player, Intersection>())
	{
		for (auto text_box_popup_spawner_entity : intersection->intersecting_entities)
		{
			if (TextPopupSpawner* text_popup_spawner = level.RawGetComponent<TextPopupSpawner>(text_box_popup_spawner_entity))
			{
				GetSingletonChildId<TextPopupSpawner>(level, text_box_popup_spawner_entity, [content = text_popup_spawner->content](Level& level) {
					auto [popup_entity, size] = CreateTexturedRectangle(level, sf::Vector2f(0, 0), sf::Vector2f(2400, 240), UI_BASE_DRAW_PRIORITY, "content\\textures\\gray.png", false);
					auto [popup_text, popup_animated_position] = level.AddComponents<Text, AnimatedPosition>(popup_entity);
					level.AddComponent<TextPopupEntityDeleteFlag>(popup_entity);
					popup_text->content = content;
					popup_text->size = 100;
					popup_animated_position->start_time = globals.time;
					popup_animated_position->animation_func = [&](float t) { return sf::Vector2f(level.GetSize().x / 2, 180 * 2 * (Ease(2 * t - 0.5, 1, 3) - 0.5)); };

					//level.AddComponent<ScheduledDelete>(popup_entity)->delete_at = globals.time + 5;
					return popup_entity;
				});
			}
		}
	}
}