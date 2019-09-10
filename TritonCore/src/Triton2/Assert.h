#pragma once

#ifdef TR_ENABLE_ASSERTS
#define TR_ASSERT(x, ...) { if(!(x)) { __debugbreak(); } }
#define TR_CORE_ASSERT(x, ...) { if(!(x)) { __debugbreak(); } }
#define TR_STATIC_ASSERT(x, ...) { static_assert(x, __VA_ARGS__); }
#else
#define TR_ASSERT(x, ...)
#define TR_CORE_ASSERT(x, ...)
#define TR_STATIC_ASSERT(x, ...)
#endif