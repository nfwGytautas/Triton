#include "TRpch.h"
#include "Renderable.h"


Triton::Core::Renderable::Renderable(std::shared_ptr<Triton::Core::Shader> aShader, std::shared_ptr<Data::Mesh> aMesh)
	: m_Shader(aShader), m_Mesh(aMesh)
{

}

void Triton::Core::Renderable::AddMaterial(MaterialEntry aMatEntry)
{
	m_Materials.push_back(aMatEntry);
}

void Triton::Core::Renderable::AddUniform(std::shared_ptr<ShaderUniforms::ShaderUniform> aUniform)
{
	m_Uniforms.push_back(aUniform);
}

void Triton::Core::Renderable::AddRenderUniform(std::shared_ptr<ShaderUniforms::ShaderUniform> aUniform)
{
	m_RenderUniforms.push_back(aUniform);
}
