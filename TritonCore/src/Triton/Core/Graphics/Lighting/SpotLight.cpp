#include "TRpch.h"
#include "SpotLight.h"

Triton::Graphics::SpotLight::SpotLight(Vector3 aPosition, Vector3 aDirection)
	:m_Position(aPosition), m_Direction(aDirection), m_CutOff(glm::cos(glm::radians(12.5f))), m_OuterCutOff(glm::cos(glm::radians(17.5f)))
{

}

Triton::Graphics::SpotLight::SpotLight(Vector3 aPosition, Vector3 aDirection, float aCutOff, float aOuterCutOff)
	: m_Position(aPosition), m_Direction(aDirection), m_CutOff(glm::cos(glm::radians(aCutOff))), m_OuterCutOff(glm::cos(glm::radians(aOuterCutOff)))
{

}

void Triton::Graphics::SpotLight::bind(relay_ptr<PType::Shader> shader)
{
	std::string head = "spotLights[" + std::to_string(Slot) + "]";

	shader->setUniformVector3(head + ".ambient", m_Ambient);
	shader->setUniformVector3(head + ".diffuse", m_Diffuse);
	shader->setUniformVector3(head + ".specular", m_Specular);

	shader->setUniformVector3(head + ".position", m_Position);
	shader->setUniformVector3(head + ".direction", m_Direction);
	shader->setUniformFloat(head + ".cutOff", m_CutOff);
	shader->setUniformFloat(head + ".outerCutOff", m_OuterCutOff);
}
