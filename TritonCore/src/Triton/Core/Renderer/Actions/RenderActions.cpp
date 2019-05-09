#include "TRpch.h"
#include "RenderActions.h"

#include <glad\glad.h>

#include "Triton\State\State.h"

void Triton::RenderActions::Prepare::Execute(relay_ptr<Singleton::State> aState)
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.0f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Triton::RenderActions::ChangeMaterial::ChangeMaterial(relay_ptr<Data::Material> aMaterial)
{
	m_Material = aMaterial;
}

void Triton::RenderActions::ChangeMaterial::Execute(relay_ptr<Singleton::State> aState)
{
	m_Material->Bind();
}

Triton::RenderActions::ChangeMesh::ChangeMesh(relay_ptr<Data::Mesh> aMesh)
{
	m_Mesh = aMesh;
}

void Triton::RenderActions::ChangeMesh::Execute(relay_ptr<Singleton::State> aState)
{
	m_Mesh->Bind();

	aState->CurrentMesh = m_Mesh;
}

Triton::RenderActions::BindShader::BindShader(relay_ptr<Core::Shader> aShader)
{
	m_Shader = aShader;
}

void Triton::RenderActions::BindShader::Execute(relay_ptr<Singleton::State> aState)
{
	m_Shader->Enable();

	aState->BindShader(m_Shader);
	aState->UpdateUniforms();
}

Triton::RenderActions::ChangeShaderUniform::ChangeShaderUniform(std::shared_ptr<ShaderUniforms::ShaderUniform> aUniform)
{
	m_Uniform = aUniform;
}

void Triton::RenderActions::ChangeShaderUniform::Execute(relay_ptr<Singleton::State> aState)
{
	aState->Shader()->Enable();
	m_Uniform->Set(aState->Shader());
}

void Triton::RenderActions::Render::Execute(relay_ptr<Singleton::State> aState)
{
	glDrawElements(GL_TRIANGLES, Singleton::State::GetInstance()->CurrentMesh->GetIndiceCount(), GL_UNSIGNED_INT, (void*)0);
}

Triton::RenderActions::BindLight::BindLight(relay_ptr<Graphics::Light> aLight)
{
	m_Light = aLight;
}

void Triton::RenderActions::BindLight::Execute(relay_ptr<Singleton::State> aState)
{
	m_Light->Bind(aState);
}

void Triton::RenderActions::UpdateUniforms::Execute(relay_ptr<Singleton::State> aState)
{
	aState->UpdateUniforms();
}
