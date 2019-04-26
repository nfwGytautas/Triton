#pragma once

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

#define TR_SHADER_UNIFORM_DEFINITION(val_type, nmspace) \
class  TR_STRINGIZE(val_type, Uniform) : public Triton::ShaderUniforms::ShaderUniform\
{\
public:\
	TR_STRINGIZE(val_type, Uniform)(std::string aName, TR_STRINGIZE(nmspace, val_type) aVal)\
		: Triton::ShaderUniforms::ShaderUniform(aName), Value(aVal) {}\
	void Set(relay_ptr<Core::Shader> aShader) override;\
\
public:\
	TR_STRINGIZE(nmspace, val_type) Value;\
};