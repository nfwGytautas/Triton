#include "TRpch.h"
#include "PlatformAbstraction.h"

#ifdef TR_PLATFORM_WINDOWS

// Temporary until DirectX is implemented
#include "OpenGL/Types.h"

Triton::PType::Context* Triton::Impl::createContext()
{
	return new PType::GLContext();
}

void Triton::Impl::destroyContext(Triton::PType::Context* context)
{
	context->shutdown();
	delete context;
}

void Triton::Impl::logErrors()
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		TR_CORE_WARN("OpenGL error: {0}", error);
	}
}

#else

#include "OpenGL/Types.h"

Triton::PType::Context * Triton::Platform::createContext()
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