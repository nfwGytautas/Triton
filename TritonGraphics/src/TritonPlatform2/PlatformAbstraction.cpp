#include "TRpch.h"
#include "PlatformAbstraction.h"

#ifdef TR_PLATFORM_WINDOWS

#include <d3d11.h>
#include <SimpleMath.h>

#include "DirectX/DXTypes.h"

void Triton::Graphics::logErrors()
{
	int error = GetLastError();
	if (error != 0)
	{

		wchar_t buf[256];
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, (sizeof(buf) / sizeof(wchar_t)), NULL);

		std::wstring ws(buf);
		std::string desc(ws.begin(), ws.end());

		TR_SYSTEM_WARN("DirectX/Win32 API error: {0} DESC: {1}", error, desc);
		TR_WARN("DirectX/Win32 API error: {0} DESC: {1}", error, desc);
	}
}

#else

#include "OpenGL/Types.h"

#endif