#include "TRpch.h"
#include "Material.h"

Triton::Data::Material::Material(std::shared_ptr<Core::Shader> aShader, std::shared_ptr<Texture> aTexture)
	: m_Shader(aShader), m_Texture(aTexture), m_Diffuse(0.0f, 0.0f, 0.0f)
{ }

void Triton::Data::Material::Bind()
{
	m_Shader->Enable();
	m_Texture->Bind(0);
	m_Shader->SetUniform("material.matTexture", 0);
}

void Triton::Data::Material::Unbind()
{
	m_Shader->Disable();
}
