#include "Context.h"

#ifdef TR_PLATFORM_WINDOWS
#include "TritonPlatform2/DirectX/Core/DXContext.h"
#define TR_RETURN_CONTEXT static_cast<Triton::Graphics::Context*>(new Triton::Graphics::DXContext())
#endif

namespace Triton
{
	namespace Graphics
	{
		Context* Context::create()
		{
			return TR_RETURN_CONTEXT;
		}

		IO::Keyboard& Context::keyboard() const
		{
			return *m_keyboardState;
		}

		IO::Mouse & Context::mouse() const
		{
			return *m_mouseState;
		}
	}
}