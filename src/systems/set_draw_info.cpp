#include "_pure_DO_systems.hpp"
#include "components/draw_info.hpp"
#include "constants.hpp"
#include "level.hpp"
#include "utils.hpp"
#include <cassert>

void SetDrawInfoSystem::Update(Level& level, float dt)
{
	for (auto const& [entity_id, charge_dependent_draw_info, charge, draw_info] : level.GetEntitiesWith<ChargeDependentDrawInfo, Charge, DrawInfo>())
	{
		int category = FindClosest(PARTICLE_CHARGE_CATEGORIES, charge->charge);
		draw_info->image_path = PARTICLE_TEXTURES[category];
	}

	for (auto const& [entity_id, orientation_dependent_drawinfo, width_and_height, draw_info] : level.GetEntitiesWith<OrientationDependentDrawInfo, WidthAndHeight, DrawInfo>())
	{
		if (width_and_height->width_and_height.x > width_and_height->width_and_height.y)
		{
			draw_info->quarter_turn_rotations = 0;
		}
		else
		{
			draw_info->quarter_turn_rotations = 1;
		}
	}

	for (auto const& [entity_id, magnetic_field, draw_info] : level.GetEntitiesWith<MagneticField, DrawInfo>())
	{
		int category = FindClosest(MAGNETIC_FIELD_STRENGTH_CATEGORIES, magnetic_field->field_strength);
		draw_info->image_path = MAGNETIC_FIELD_TEXTURES[category];
	}

	for (auto const& [entity_id, electric_field, draw_info] : level.GetEntitiesWith<ElectricField, DrawInfo>())
	{
		int category = FindClosest(ELECTRIC_FIELD_STRENGTH_CATEGORIES, Magnitude(electric_field->field_vector));
		draw_info->image_path = ELECTRIC_FIELD_TEXTURES[category];
	}
}