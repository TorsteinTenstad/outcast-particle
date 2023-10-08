#include "components/button.hpp"
#include "components/button_events.hpp"
#include "components/draw_info.hpp"
#include "components/sound_info.hpp"
#include "components/text.hpp"
#include "systems/_pure_DO_systems.hpp"

void CanDisableButtonSystem::Update(Level& level, float dt)
{
	for (auto& [entity, button] : level.GetEntitiesWith<CanDisableButton>())
	{
		bool enabled = button->func();
		if (Text* text = level.RawGetComponent<Text>(entity))
		{
			text->color.a = enabled ? 255 : button->deactivation_alpha;
		}
		if (FillColor* fill_color = level.RawGetComponent<FillColor>(entity))
		{
			fill_color->color.a = enabled ? 255 : button->deactivation_alpha;
		}
		if (enabled)
		{
			if (button->regain_button_events)
			{
				level.EnsureExistenceOfComponent<ReceivesButtonEvents>(entity);
			}
		}
		else
		{
			level.RemoveComponent<ReceivesButtonEvents>(entity);
		}
	}
}