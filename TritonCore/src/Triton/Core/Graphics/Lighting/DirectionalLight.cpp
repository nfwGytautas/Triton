#include "TRpch.h"
#include "DirectionalLight.h"

#include "Triton\State\State.h"

Triton::Graphics::DirectionalLight::DirectionalLight(Vector3 aDirection)
	:m_Direction(aDirection)
{
}

void Triton::Graphics::DirectionalLight::Bind(relay_ptr<Singleton::State> aState)
{
	unsigned int slot = aState->AddLight(this);

	auto shader = aState->Shader();

	std::string head = "dirLights[" + std::to_string(slot) + "]";

	shader->Enable();

	shader->SetUniform(head + ".ambient", m_Ambient);
	shader->SetUniform(head + ".diffuse", m_Diffuse);
	shader->SetUniform(head + ".specular", m_Specular);

	shader->SetUniform(head + ".direction", m_Direction);

	shader->Disable();
}
