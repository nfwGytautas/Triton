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

#define BIT(x) (1 << x)

#ifdef TR_PLATFORM_WINDOWS

#else
	
#endif

#define RANGED_FOR(x,y) for(int i = 0; i < x; i++) { y }

#define TR_STRINGIZE(A, B) A ## B

#define TR_BIND_FUNC(func, obj, ...) std::bind(&func, obj, __VA_ARGS__)