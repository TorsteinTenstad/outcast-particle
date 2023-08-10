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
		if (button->func())
		{
			level.EnsureExistenceOfComponent<ReceivesButtonEvents>(entity);
			level.GetComponent<Text>(entity)->color.a = 255;
			level.GetComponent<FillColor>(entity)->color.a = 255;
		}
		else
		{
			level.RemoveComponent<ReceivesButtonEvents>(entity);
			level.GetComponent<Text>(entity)->color.a = 50;
			level.GetComponent<FillColor>(entity)->color.a = 50;
		}
	}
}