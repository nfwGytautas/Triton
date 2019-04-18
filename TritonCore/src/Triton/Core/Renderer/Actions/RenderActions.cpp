#include "TRpch.h"
#include "RenderActions.h"

#include <glad\glad.h>

void Triton::RenderActions::Prepare::Execute()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.0f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Triton::RenderActions::ChangeMaterial::ChangeMaterial(std::shared_ptr<Data::Material> aMaterial)
{
	m_Material = aMaterial;
}

void Triton::RenderActions::ChangeMaterial::Execute()
{
	m_Material->Bind();
}

Triton::RenderActions::ChangeMesh::ChangeMesh(std::shared_ptr<Data::Mesh> aMesh)
{
	m_Mesh = aMesh;
}

void Triton::RenderActions::ChangeMesh::Execute()
{
	m_Mesh->Bind();
}

Triton::RenderActions::BindShader::BindShader(std::shared_ptr<Core::Shader> aShader)
{
	m_Shader = aShader;
}

void Triton::RenderActions::BindShader::Execute()
{
	m_Shader->Enable();
}

Triton::RenderActions::ChangeShaderUniform::ChangeShaderUniform(std::shared_ptr<Core::Shader> aShader, std::shared_ptr<ShaderUniforms::ShaderUniform> aUniform)
{
	m_Uniform = aUniform;
	m_Shader = aShader;
}

void Triton::RenderActions::ChangeShaderUniform::Execute()
{
	m_Shader->Enable();
	m_Uniform->Set(m_Shader);
}

Triton::RenderActions::Render::Render(unsigned int aIndiceCount)
{
	m_IndiceCount = aIndiceCount;
}

void Triton::RenderActions::Render::Execute()
{
	glDrawElements(GL_TRIANGLES, m_IndiceCount, GL_UNSIGNED_INT, (void*)0);
}
