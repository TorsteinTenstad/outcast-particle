
#include "components/draw_info.hpp"
#include "components/laser.hpp"
#include "globals.hpp"
#include "systems/_pure_DO_systems.hpp"
#include <cmath>

void TogglingLaserSystem::Update(Level& level, float dt)
{
	if (level.GetMode() == PAUSE_MODE) { return; }
	for (const auto& [entity, toggling_laser, laser, fill_color] : level.GetEntitiesWith<TogglingLaser, Laser, FillColor>())
	{
		assert(0 <= toggling_laser->duty_cycle && toggling_laser->duty_cycle <= 1);
		bool is_on = std::fmod(globals.time + toggling_laser->phase, toggling_laser->period_s) / toggling_laser->period_s < toggling_laser->duty_cycle;
		laser->kill_on_intersection_enabled = is_on;
		fill_color->color.a = is_on ? 255 : 40;
	}
}