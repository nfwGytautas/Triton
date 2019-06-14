#include "TRpch.h"
#include "DirectionalLight.h"

Triton::Graphics::DirectionalLight::DirectionalLight(Vector3 aDirection)
	:m_Direction(aDirection)
{
}

void Triton::Graphics::DirectionalLight::bind(relay_ptr<PType::Shader> shader)
{
	std::string head = "dirLights[" + std::to_string(Slot) + "]";

	auto diffuse = Triton::Vector4(Diffuse.x, Diffuse.y, Diffuse.z, 1.0f);
	auto ambient = Triton::Vector4(Ambient.x, Ambient.y, Ambient.z, 1.0f);
	auto direction = m_Direction;

	shader->setBufferValue("LightBuffer", "diffuseColor", &diffuse);
	shader->setBufferValue("LightBuffer", "ambientColor", &ambient);
	shader->setBufferValue("LightBuffer", "lightDirection", &direction);
	//shader->setUniformVector3(head + ".ambient", m_Ambient);
	//shader->setUniformVector3(head + ".diffuse", m_Diffuse);
	//shader->setUniformVector3(head + ".specular", m_Specular);
	//
	//shader->setUniformVector3(head + ".direction", m_Direction);
}