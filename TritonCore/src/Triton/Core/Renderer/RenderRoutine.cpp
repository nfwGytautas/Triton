#include "TRpch.h"
#include "RenderRoutine.h"

void Triton::Core::RenderRoutine::Bind(std::shared_ptr<Core::Shader> aShader)
{
	SetUniforms(*aShader.get());
}
