#include "components/open_file_dialog.hpp"
#include "platform_specific/platform_specific.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "windows.h"
#include <codecvt>
#include <locale>
#include <shobjidl.h>
#include <sstream>
#include <string>

void OpenFileDialogSystem::Update(Level& level, float dt)
{
	for (const auto [entity, open_file_dialog] : level.GetEntitiesWith<OpenFileDialog>())
	{
		if (!open_file_dialog->show) { continue; }
		open_file_dialog->path = GetPathFromOpenFileDialog();
		open_file_dialog->show = false;
	}
}