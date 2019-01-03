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
	#define TR_GLEW_INIT GLenum err = glewInit(); \
						 if (err != GLEW_OK)\
						 {\
						 	 TR_CORE_ERROR("GLEW Initialization error '{0}'", glewGetErrorString(err));\
						 	 throw "GLEW INITIALIZATION FAILURE";\
						 }\
						 else\
						 {\
						 	 TR_CORE_INFO("GLEW version: {0} initialization successful", glewGetString(GLEW_VERSION));\
						 }
#else
	#define TR_GLEW_INIT 
#endif

//Project typedefs
namespace Triton
{
	namespace Data
	{
		class Mesh;
	}
	TRITON_API typedef unsigned int MeshID;
	TRITON_API typedef std::vector<std::shared_ptr<Data::Mesh>> MeshList;
}