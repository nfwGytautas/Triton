#include "TRpch.h"
#include "Shader.h"

#define SET_DECLARATION(type)\
void Triton::ShaderUniforms::TR_STRINGIZE(type, Uniform)::Set(std::shared_ptr<Triton::Core::Shader> aShader)\
{\
	aShader->SetUniform(Name, Value);\
}\

SET_DECLARATION(int)
SET_DECLARATION(float)
SET_DECLARATION(Vector2)
SET_DECLARATION(Vector3)
SET_DECLARATION(Vector4)
SET_DECLARATION(Matrix44)