#include "entity_creation.hpp"
#include "globals.hpp"
#include "systems/_pure_DO_systems.hpp"

void ShowErrorSystem::Update(Level& level, float dt)
{
	if (globals.errors.corrupt_files.size() > 0)
	{
		std::string message = "Failed to load game files correctly, consider re-installing.";
		for (const std::string& filename : globals.errors.corrupt_files)
		{
			message += "\n\t" + filename;
		}
		globals.errors.corrupt_files.clear();
		globals.errors.error_messages_to_display.push_back(message);
	}

	while (globals.errors.error_messages_to_display.size() > 0)
	{
		std::string message = globals.errors.error_messages_to_display.front();
		globals.errors.error_messages_to_display.pop_front();
		CreateBlockingInformationMenu(level, level.GetSize(), "", message);
	}
}
