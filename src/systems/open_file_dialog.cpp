#include "components/open_file_dialog.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "windows.h"
#include <shobjidl.h>
#include <sstream>

void OpenFileDialogSystem::Update(Level& level, float dt)
{
	for (const auto [entity, open_file_dialog] : level.GetEntitiesWith<OpenFileDialog>())
	{
		if (open_file_dialog->path.has_value()) { continue; }

		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (SUCCEEDED(hr))
		{
			IFileOpenDialog* pFileOpen;

			// Create the FileOpenDialog object.
			hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

			if (SUCCEEDED(hr))
			{
				// Show the Open dialog box.
				hr = pFileOpen->Show(NULL);

				// Get the file name from the dialog box.
				if (SUCCEEDED(hr))
				{
					IShellItem* pItem;
					hr = pFileOpen->GetResult(&pItem);
					if (SUCCEEDED(hr))
					{
						PWSTR pszFilePath;
						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

						// Display the file name to the user.
						if (SUCCEEDED(hr))
						{
							MessageBoxW(NULL, pszFilePath, L"File Path", MB_OK);
							CoTaskMemFree(pszFilePath);

							std::stringstream s;
							s << pszFilePath;
							open_file_dialog->path = s.str();
						}
						pItem->Release();
					}
				}
				pFileOpen->Release();
			}
			CoUninitialize();
		}
	}
}