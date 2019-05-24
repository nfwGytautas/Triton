#include "TRpch.h"
#include "DirectionalLight.h"

Triton::Graphics::DirectionalLight::DirectionalLight(Vector3 aDirection)
	:m_Direction(aDirection)
{
}

void Triton::Graphics::DirectionalLight::bind(relay_ptr<PType::Shader> shader)
{
	std::string head = "dirLights[" + std::to_string(Slot) + "]";

	shader->setUniformVector3(head + ".ambient", m_Ambient);
	shader->setUniformVector3(head + ".diffuse", m_Diffuse);
	shader->setUniformVector3(head + ".specular", m_Specular);

	shader->setUniformVector3(head + ".direction", m_Direction);
}
