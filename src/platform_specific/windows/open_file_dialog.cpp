#include "platform_specific/platform_specific.hpp"
#include "windows.h"
#include <codecvt>
#include <locale>
#include <optional>
#include <shobjidl.h>
#include <sstream>
#include <string>

std::optional<std::string> GetPathFromOpenFileDialog()
{
	std::optional<std::string> result = std::nullopt;
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
						std::wstringstream ws;
						ws << pszFilePath;
						std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
						result = converter.to_bytes(ws.str());
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}
	return result;
}