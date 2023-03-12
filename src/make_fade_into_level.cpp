#include "make_fade_into_level.hpp"
#include "utils.hpp"

void MakeFadeIntoLevel(Level& level, int entity_id, std::string active_level_id)
{
	if (!IsMenu(active_level_id))
	{
		level.AddComponent<FillColor>(entity_id);
		AnimatedOpacity* animated_opacity = level.AddComponent<AnimatedOpacity>(entity_id);
		animated_opacity->animation_func = [](float t) { return sf::Uint8(255 * Smoothstep(2 * t - 1)); };
		animated_opacity->start_time = globals.time_of_last_level_enter;
	}
}