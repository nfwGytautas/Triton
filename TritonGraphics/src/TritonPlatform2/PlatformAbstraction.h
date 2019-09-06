#pragma once

// Check the current platform and get the specific abstracted function definitions
// that can be used in a cross-platform manner.

#ifdef TR_PLATFORM_WINDOWS

#define NOMINMAX

#else


#endif


// Include all shared types between all platforms.
#include "CrossTypes/Core/Context.h"
#include "mathematical.h"