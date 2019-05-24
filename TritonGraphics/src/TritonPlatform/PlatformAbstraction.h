#pragma once

#include "TritonTypes/mathematical.h"

// Check the current platform and get the specific abstracted function definitions
// that can be used in a cross-platform manner.

#ifdef TR_PLATFORM_WINDOWS

// Temporary until DirectX is implemented
#include "OpenGL/Specific.h"

#else


#endif


// Include all shared types between all platforms.

#include "CrossTypes/Types.h"


// Functions that aren't specific and nor hardware nor the platform must be defined.
namespace Triton
{
	namespace Impl
	{
		Triton::PType::Context* createContext();
		void destroyContext(Triton::PType::Context* context);
		void logErrors();
	}
}