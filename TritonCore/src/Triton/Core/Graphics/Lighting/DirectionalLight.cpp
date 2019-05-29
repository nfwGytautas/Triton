#include "TRpch.h"
#include "DirectionalLight.h"

Triton::Graphics::DirectionalLight::DirectionalLight(Vector3 aDirection)
	:m_Direction(aDirection)
{
}

void Triton::Graphics::DirectionalLight::bind(relay_ptr<PType::Shader> shader)
{
	std::string head = "dirLights[" + std::to_string(Slot) + "]";

	shader->setUniformVector4("diffuseColor", Triton::Vector4(Diffuse.x, Diffuse.y, Diffuse.z, 1.0f));
	shader->setUniformVector4("ambientColor", Triton::Vector4(Ambient.x, Ambient.y, Ambient.z, 1.0f));
	shader->setUniformVector3("lightDirection", m_Direction);
	//shader->setUniformVector3(head + ".ambient", m_Ambient);
	//shader->setUniformVector3(head + ".diffuse", m_Diffuse);
	//shader->setUniformVector3(head + ".specular", m_Specular);
	//
	//shader->setUniformVector3(head + ".direction", m_Direction);
}
