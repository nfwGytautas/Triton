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

void Triton::Graphics::SpotLight::Bind(Core::Shader& aShader)
{
	aShader.Enable();

	aShader.SetUniform("pointLights[0].ambient", m_Ambient);
	aShader.SetUniform("pointLights[0].diffuse", m_Diffuse);
	aShader.SetUniform("pointLights[0].specular", m_Specular);

	aShader.SetUniform("light.position", m_Position);
	aShader.SetUniform("light.direction", m_Position);
	aShader.SetUniform("light.cutOff", m_CutOff);
	aShader.SetUniform("light.outerCutOff", m_OuterCutOff);
}
