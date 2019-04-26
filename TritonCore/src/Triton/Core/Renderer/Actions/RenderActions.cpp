#include "TRpch.h"
#include "RenderActions.h"

#include <glad\glad.h>

#include "Triton\State\State.h"

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

	Singleton::State::GetInstance()->CurrentMesh = m_Mesh;
}

Triton::RenderActions::BindShader::BindShader(std::shared_ptr<Core::Shader> aShader)
{
	m_Shader = aShader;
}

void Triton::RenderActions::BindShader::Execute()
{
	m_Shader->Enable();

	Singleton::State::GetInstance()->BindShader(m_Shader);
}

Triton::RenderActions::ChangeShaderUniform::ChangeShaderUniform(std::shared_ptr<ShaderUniforms::ShaderUniform> aUniform)
{
	m_Uniform = aUniform;
}

void Triton::RenderActions::ChangeShaderUniform::Execute()
{
	Singleton::State::GetInstance()->Shader()->Enable();
	m_Uniform->Set(Singleton::State::GetInstance()->Shader());
}

void Triton::RenderActions::Render::Execute()
{
	glDrawElements(GL_TRIANGLES, Singleton::State::GetInstance()->CurrentMesh->GetIndiceCount(), GL_UNSIGNED_INT, (void*)0);
}

Triton::RenderActions::BindLight::BindLight(std::shared_ptr<Graphics::Light> aLight)
{
	m_Light = aLight;
}

void Triton::RenderActions::BindLight::Execute()
{
	m_Light->Bind(Singleton::State::GetInstance());
}

void Triton::RenderActions::UpdateUniforms::Execute()
{
	Singleton::State::GetInstance()->UpdateUniforms();
}
