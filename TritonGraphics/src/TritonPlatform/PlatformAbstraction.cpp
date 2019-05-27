#include "TRpch.h"
#include "PlatformAbstraction.h"

#ifdef TR_PLATFORM_WINDOWS

#include "DirectX/Types.h"

Triton::PType::Context* Triton::Impl::createContext(const Triton::AppSettings& appSettings)
{
	return new PType::DXContext(appSettings.WindowTitle);
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
		TR_CORE_WARN("DirectX/Win32 API error: {0}", error);
	}
}

std::wstring Triton::Impl::s2ws(const std::string & s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
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