#pragma once

//DLL API MACRO
#ifdef TR_SCRIPTING_BUILD_DLL
	#define TRITON_SCRIPTING_API __declspec(dllexport)
#else
	#define TRITON_SCRIPTING_API __declspec(dllimport)
#endif