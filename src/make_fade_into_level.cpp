#include "make_fade_into_level.hpp"
#include "components/animated_properties.hpp"
#include "components/creation_data.hpp"
#include "components/draw_info.hpp"
#include "utils/level_id.hpp"
#include "utils/math.hpp"

void MakeFadeIntoLevel(Level& level, Entity entity, std::string active_level_id)
{
	if (!IsMenu(active_level_id) && level.GetMode() != EDIT_MODE)
	{
		level.EnsureExistenceOfComponent<FillColor>(entity)->color = sf::Color(255, 255, 255, 0);
		AnimatedOpacity* animated_opacity = level.AddComponent<AnimatedOpacity>(entity);
		animated_opacity->animation_func = [](float t) { return sf::Uint8(255 * Smoothstep(3 * t - 0.75)); };
		animated_opacity->start_time = globals.time;
	}
}