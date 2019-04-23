#include "TRpch.h"
#include "Material.h"

Triton::Data::Material::Material(std::shared_ptr<Core::Shader> aShader, std::shared_ptr<Texture> aTexture)
	: m_Shader(aShader), m_Texture(aTexture), m_Ambient(1.0f, 1.0f, 1.0f), m_Diffuse(1.0f, 1.0f, 1.0f), m_Specular(1.0f, 1.0f, 1.0f), m_Shininess(32.0f)
{ }

void Triton::Data::Material::Bind()
{
	m_Shader->Enable();
	m_Texture->Bind(0);
	m_Shader->SetUniform("material.matTexture", 0);
	m_Shader->SetUniform("material.ambient", m_Ambient);
	m_Shader->SetUniform("material.diffuse", m_Diffuse);
	m_Shader->SetUniform("material.specular", m_Specular);
	m_Shader->SetUniform("material.shininess", m_Shininess);
}

void Triton::Data::Material::Unbind()
{
	m_Shader->Disable();
}
