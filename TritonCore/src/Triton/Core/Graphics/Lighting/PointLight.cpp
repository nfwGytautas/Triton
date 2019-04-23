#include "TRpch.h"
#include "PointLight.h"

Triton::Graphics::PointLight::PointLight(Vector3 aPosition)
	:m_Position(aPosition), m_Constant(1.0f), m_Linear(0.09f), m_Quadratic(0.032f)
{
}

Triton::Graphics::PointLight::PointLight(Vector3 aPosition, float aConstant, float aLinear, float aQuadratic)
	: m_Position(aPosition), m_Constant(aConstant), m_Linear(aLinear), m_Quadratic(aQuadratic)
{
}

void Triton::Graphics::PointLight::Bind(Core::Shader& aShader)
{
	aShader.Enable();

	aShader.SetUniform("pointLights[0].ambient", m_Ambient);
	aShader.SetUniform("pointLights[0].diffuse", m_Diffuse);
	aShader.SetUniform("pointLights[0].specular", m_Specular);

	aShader.SetUniform("pointLights[0].position", m_Position);
	aShader.SetUniform("pointLights[0].constant", m_Constant);
	aShader.SetUniform("pointLights[0].linear", m_Linear);
	aShader.SetUniform("pointLights[0].quadratic", m_Quadratic);

	aShader.SetUniform("num_of_pointlights", 1);

}