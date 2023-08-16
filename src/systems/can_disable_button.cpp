#include "components/button.hpp"
#include "components/button_events.hpp"
#include "components/draw_info.hpp"
#include "components/sound_info.hpp"
#include "components/text.hpp"
#include "systems/_pure_DO_systems.hpp"

void CanDisableButtonSystem::Update(Level& level, float dt)
{
	for (auto& [entity, button, fill_color] : level.GetEntitiesWith<CanDisableButton, FillColor>())
	{
		if (button->func())
		{
			if (level.HasComponents<OnReleasedThisFrame>(entity))
			{
				level.EnsureExistenceOfComponent<ReceivesButtonEvents>(entity);
			}
			fill_color->color.a = 255;

			if (Text* text = level.RawGetComponent<Text>(entity))
			{
				text->color.a = 255;
			}
		}
		else
		{
			level.RemoveComponent<ReceivesButtonEvents>(entity);
			fill_color->color.a = button->deactivation_alpha;

			if (Text* text = level.RawGetComponent<Text>(entity))
			{
				text->color.a = button->deactivation_alpha;
			}
		}
	}
}