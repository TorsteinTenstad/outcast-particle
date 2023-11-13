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
#include "utils/string_manip.hpp"
#include "utils/string_parsing.hpp"

class TextPopupEntity
{};

void TextPopupSystem::Update(Level& level, float dt)
{
	for (auto [entity, text_popup_spawner, draw_info] : level.GetEntitiesWith<TextPopupSpawner, DrawInfo>())
	{
		draw_info->image_path = level.GetMode() == EDIT_MODE ? (TEXTURES_DIR / "white.png").string() : (TEXTURES_DIR / "transparent.png").string();
		bool player_is_intersecting = false;
		for (auto [player_entity, player, intersection] : level.GetEntitiesWith<Player, Intersection>())
		{
			for (auto intersecting_entity : intersection->intersecting_entities)
			{
				if (intersecting_entity == entity)
				{
					player_is_intersecting = true;
					break;
				}
			}
		}
		if (cursor_and_keys_.key_pressed_this_frame[text_popup_spawner->key])
		{
			text_popup_spawner->alert_timer = -5.5;
		}
		if (player_is_intersecting)
		{
			text_popup_spawner->alert_timer += dt;
			auto content_parts = SplitString(text_popup_spawner->content, "_");
			auto popup_entity = level.GetSingletonId<TextPopupEntity>([content = content_parts.front() + HumanName(text_popup_spawner->key) + (content_parts.size() > 1 ? content_parts.back() : ""),
																		  level_center = (level.GetSize().x / 2)](ECSScene& level) {
				auto [popup_entity, size] = CreateTexturedRectangle(level, sf::Vector2f(level_center, 1.5 * BLOCK_SIZE), sf::Vector2f(2400, 240), UI_BASE_DRAW_PRIORITY, (TEXTURES_DIR / "gray.png").string(), false);
				auto [popup_text, animated_opacity, shader] = level.AddComponents<Text, AnimatedOpacity, Shader>(popup_entity);
				popup_text->content = content;
				popup_text->size = 120;
				animated_opacity->start_time = globals.time;
				animated_opacity->animation_func = [level_center](float t) { return (sf::Uint8)(255 * Ease(t, 0, 0)); };
				shader->fragment_shader_path = (SHADERS_DIR / "text_popup.frag").string();
				//level.AddComponent<ScheduledDelete>(popup_entity)->delete_at = globals.time + 5;
				return popup_entity;
			});
			if (text_popup_spawner->alert_timer > 2.5)
			{
				text_popup_spawner->alert_timer = 0;
				level.GetComponent<Shader>(popup_entity)->float_uniforms["animation_start_time"] = globals.time;
			}
		}
		else
		{
			text_popup_spawner->alert_timer = 0;
		}
	}
	if (level.GetMode() != PLAY_MODE || level.ComputeState() != PLAYING)
	{
		level.DeleteEntitiesWith<TextPopupEntity>();
		return;
	}
}