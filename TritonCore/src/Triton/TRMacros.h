#pragma once

//DLL API MACRO
#ifdef TR_BUILD_DLL
	#define TRITON_API __declspec(dllexport)
#else
	#define TRITON_API __declspec(dllimport)
#endif

#ifdef TR_ENABLE_ASSERTS
	#include "Triton\Logger\Log.h"
	#define TR_ASSERT(x, ...) { if(!(x)) { TR_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define TR_CORE_ASSERT(x, ...) { if(!(x)) { TR_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
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

//Project typedefs
namespace Triton
{
	namespace Data
	{
		class Mesh;
		class Texture;
	}
	TRITON_API typedef unsigned int MeshID;
	TRITON_API typedef std::vector<std::shared_ptr<Data::Mesh>> MeshList;

	TRITON_API typedef unsigned int TextureID;
	TRITON_API typedef std::vector<std::shared_ptr<Data::Texture>> TextureList;
}