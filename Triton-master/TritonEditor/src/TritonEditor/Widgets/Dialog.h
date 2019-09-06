#pragma once

#include <shobjidl_core.h>
#include <tchar.h>

enum class FileFilter
{
	nill = -1,
	Project = 0,
};

LPWSTR selectFolder()
{
	LPWSTR path;

	IFileDialog *pfd;
	if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd))))
	{
		DWORD dwOptions;
		if (SUCCEEDED(pfd->GetOptions(&dwOptions)))
		{
			pfd->SetOptions(dwOptions | FOS_PICKFOLDERS | FOS_NOCHANGEDIR);
		}
		if (SUCCEEDED(pfd->Show(NULL)))
		{
			IShellItem *psi;
			if (SUCCEEDED(pfd->GetResult(&psi)))
			{
				if (!SUCCEEDED(psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &path)))
				{
					TR_ERROR("Error occurred when trying to extract folder path");
				}
				psi->Release();
			}
		}
		pfd->Release();
	}

	return path;
}

LPWSTR selectFile(FileFilter filter = FileFilter::nill)
{
	LPWSTR path;

	IFileDialog *pfd;
	if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd))))
	{
		DWORD dwOptions;
		if (SUCCEEDED(pfd->GetOptions(&dwOptions)))
		{
			pfd->SetOptions(dwOptions | FOS_NOCHANGEDIR);

			if(filter == FileFilter::Project)
			{
				COMDLG_FILTERSPEC rgSpec[] =
				{
					{ L"Triton Project", L"*.tproj" }
				};

				pfd->SetFileTypes(1, rgSpec);
			}
		}
		if (SUCCEEDED(pfd->Show(NULL)))
		{
			IShellItem *psi;
			if (SUCCEEDED(pfd->GetResult(&psi)))
			{
				if (!SUCCEEDED(psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &path)))
				{
					TR_ERROR("Error occurred when trying to extract file path");
				}
				psi->Release();
			}
		}
		else
		{
			return _T("");
		}
		pfd->Release();
	}

	return path;
}