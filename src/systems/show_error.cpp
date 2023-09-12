#include "components/blocking_popup_menu_entity.hpp"
#include "entity_creation.hpp"
#include "globals.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "utils/string_parsing.hpp"

void ShowErrorSystem::Update(Level& level, float dt)
{
	if (globals.errors.error_infos.size() == 0) { return; }
	if (level.GetEntitiesWithComponent<BlockingPopupMenuEntity>().size() > 0) { return; }
	ErrorInfo error_info = globals.errors.error_infos.front();
	globals.errors.error_infos.pop_front();
	std::string title = "Error " + ToString(static_cast<int>(error_info.error_number));
	CreateBlockingInformationMenu(level, level.GetSize(), title, error_info.message.value_or(""), level.GetScale());
}
