#include "TRpch.h"
#include "SpotLight.h"

#include "Triton\State\State.h"

Triton::Graphics::SpotLight::SpotLight(Vector3 aPosition, Vector3 aDirection)
	:m_Position(aPosition), m_Direction(aDirection), m_CutOff(glm::cos(glm::radians(12.5f))), m_OuterCutOff(glm::cos(glm::radians(17.5f)))
{

}

Triton::Graphics::SpotLight::SpotLight(Vector3 aPosition, Vector3 aDirection, float aCutOff, float aOuterCutOff)
	: m_Position(aPosition), m_Direction(aDirection), m_CutOff(glm::cos(glm::radians(aCutOff))), m_OuterCutOff(glm::cos(glm::radians(aOuterCutOff)))
{

}

void Triton::Graphics::SpotLight::Bind(relay_ptr<Singleton::State> aState)
{
	unsigned int slot = aState->AddLight(this);

	auto shader = aState->Shader();

	std::string head = "spotLights[" + std::to_string(slot) + "]";

	shader->Enable();

	shader->SetUniform(head + ".ambient", m_Ambient);
	shader->SetUniform(head + ".diffuse", m_Diffuse);
	shader->SetUniform(head + ".specular", m_Specular);

	shader->SetUniform(head + ".position", m_Position);
	shader->SetUniform(head + ".direction", m_Direction);
	shader->SetUniform(head + ".cutOff", m_CutOff);
	shader->SetUniform(head + ".outerCutOff", m_OuterCutOff);

	shader->Disable();
}
