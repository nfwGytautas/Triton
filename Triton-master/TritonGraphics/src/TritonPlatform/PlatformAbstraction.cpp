#include "TRpch.h"
#include "PlatformAbstraction.h"

#ifdef TR_PLATFORM_WINDOWS

#include <d3d11.h>
#include <SimpleMath.h>

#include "DirectX/DXTypes.h"

Triton::PType::Context* Triton::Impl::createContext(const Triton::AppSettings& appSettings)
{
	return new PType::DXContext(appSettings);
}

void Triton::Impl::destroyContext(Triton::PType::Context* context)
{
	context->shutdown();
	delete context;
}

void Triton::Impl::logErrors()
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

Triton::PType::Context * Triton::Platform::createContext(const Triton::AppSettings& appSettings)
{
	return new PType::GLContext();
}

void Triton::Impl::logErrors()
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		TR_CORE_WARN("OpenGL error: {0}", error);
	}
}

#endif