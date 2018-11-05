#pragma once

//DLL API MACRO
#ifdef TR_BUILD_DLL
	#define TRITON_API __declspec(dllexport)
#else
	#define TRITON_API __declspec(dllimport)
#endif