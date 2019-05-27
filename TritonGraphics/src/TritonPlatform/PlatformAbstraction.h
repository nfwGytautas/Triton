#pragma once

// Check the current platform and get the specific abstracted function definitions
// that can be used in a cross-platform manner.

#ifdef TR_PLATFORM_WINDOWS

#define NOMINMAX
#include "DirectX/Specific.h"
#include <d3d11.h>
#include <SimpleMath.h>


#else

#include "OpenGL/Specific.h"

#endif


// Include all shared types between all platforms.
#include "CrossTypes/Types.h"
#include "Triton/AppSettings.h"
#include "mathematical.h"

// Functions that aren't specific and nor hardware nor the platform must be defined.
namespace Triton
{
	namespace Impl
	{
		Triton::PType::Context* createContext(const Triton::AppSettings& appSettings);
		void destroyContext(Triton::PType::Context* context);
		void logErrors();
		std::wstring s2ws(const std::string& s);
	}
}