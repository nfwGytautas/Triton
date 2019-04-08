#include "TRpch.h"
#include "UniformHost.h"

void Triton::Core::UniformHost::AddUniform(std::shared_ptr<ShaderUniforms::ShaderUniform> aUniform)
{
	m_Uniforms.push_back(aUniform);
}

void Triton::Core::UniformHost::SetUniforms(Shader& aShader)
{
	for (auto& uniform : m_Uniforms)
	{
		uniform->Set(aShader);
	}
}
