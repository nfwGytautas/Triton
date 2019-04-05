#include "TRpch.h"
#include "Shader.h"

#define SET_DECLARATION(type)\
void Triton::ShaderUniforms::TR_STRINGIZE(type, Uniform)::Set(Triton::Core::Shader& aShader)\
{\
	TR_CORE_ASSERT(m_Value, "Shader value is null");\
	aShader.SetUniform(m_Name, *m_Value);\
}\

SET_DECLARATION(int)
SET_DECLARATION(float)
SET_DECLARATION(Vector2)
SET_DECLARATION(Vector3)
SET_DECLARATION(Vector4)
SET_DECLARATION(Matrix44)