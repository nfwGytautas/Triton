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

void Triton::Graphics::PointLight::bind(relay_ptr<PType::Shader> shader)
{
	std::string head = "pointLights[" + std::to_string(Slot) + "]";

	//shader->setUniformVector3(head + ".ambient", m_Ambient);
	//shader->setUniformVector3(head + ".diffuse", m_Diffuse);
	//shader->setUniformVector3(head + ".specular", m_Specular);
	//
	//shader->setUniformVector3(head + ".position", m_Position);
	//shader->setUniformFloat(head + ".constant", m_Constant);
	//shader->setUniformFloat(head + ".linear", m_Linear);
	//shader->setUniformFloat(head + ".quadratic", m_Quadratic);
}