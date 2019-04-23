#include "TRpch.h"
#include "DirectionalLight.h"

Triton::Graphics::DirectionalLight::DirectionalLight(Vector3 aDirection)
	:m_Direction(aDirection)
{
}

void Triton::Graphics::DirectionalLight::Bind(Core::Shader& aShader)
{
	aShader.Enable();

	aShader.SetUniform("pointLights[0].ambient", m_Ambient);
	aShader.SetUniform("pointLights[0].diffuse", m_Diffuse);
	aShader.SetUniform("pointLights[0].specular", m_Specular);

	aShader.SetUniform("light.direction", m_Direction);
}
