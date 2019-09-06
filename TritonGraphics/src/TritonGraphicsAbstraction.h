#pragma once

#define TR_PLATFORM_ABSTRACTION_VERSION 2

#if TR_PLATFORM_ABSTRACTION_VERSION == 1
#include "TritonPlatform/PlatformAbstraction.h"
#endif

#if TR_PLATFORM_ABSTRACTION_VERSION == 2
#include "TritonPlatform2/PlatformAbstraction.h"
#endif