#include "TRpch.h"
#include "RenderActions.h"

#include <glad\glad.h>

void Triton::RenderActions::Prepare::Execute(Core::ContextState& aState)
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

void Triton::RenderActions::ChangeMaterial::Execute(Core::ContextState& aState)
{
	m_Material->Bind();
}

Triton::RenderActions::ChangeMesh::ChangeMesh(std::shared_ptr<Data::Mesh> aMesh)
{
	m_Mesh = aMesh;
}

void Triton::RenderActions::ChangeMesh::Execute(Core::ContextState& aState)
{
	m_Mesh->Bind();
	aState.BoundMesh = m_Mesh;
}

Triton::RenderActions::BindShader::BindShader(std::shared_ptr<Core::Shader> aShader)
{
	m_Shader = aShader;
}

void Triton::RenderActions::BindShader::Execute(Core::ContextState& aState)
{
	m_Shader->Enable();
	aState.BoundShader = m_Shader;
}

Triton::RenderActions::ChangeShaderUniform::ChangeShaderUniform(std::shared_ptr<ShaderUniforms::ShaderUniform> aUniform)
{
	m_Uniform = aUniform;
}

void Triton::RenderActions::ChangeShaderUniform::Execute(Core::ContextState& aState)
{
	aState.BoundShader->Enable();
	m_Uniform->Set(aState.BoundShader);
}

void Triton::RenderActions::Render::Execute(Core::ContextState& aState)
{
	glDrawElements(GL_TRIANGLES, aState.BoundMesh->GetIndiceCount(), GL_UNSIGNED_INT, (void*)0);
}

Triton::RenderActions::BindLight::BindLight(std::shared_ptr<Graphics::Light> aLight)
{
	m_Light = aLight;
}

void Triton::RenderActions::BindLight::Execute(Core::ContextState& aState)
{
	m_Light->Bind(*aState.BoundShader.get());
}
