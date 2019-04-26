#include "TRpch.h"
#include "PointLight.h"

#include "Triton\State\State.h"

Triton::Graphics::PointLight::PointLight(Vector3 aPosition)
	:m_Position(aPosition), m_Constant(1.0f), m_Linear(0.09f), m_Quadratic(0.032f)
{
}

Triton::Graphics::PointLight::PointLight(Vector3 aPosition, float aConstant, float aLinear, float aQuadratic)
	: m_Position(aPosition), m_Constant(aConstant), m_Linear(aLinear), m_Quadratic(aQuadratic)
{
}

void Triton::Graphics::PointLight::Bind(relay_ptr<Singleton::State> aState)
{
	unsigned int slot = aState->AddLight(this);

	auto shader = aState->Shader();

	std::string head = "pointLights[" + std::to_string(slot) + "]";

	shader->Enable();

	shader->SetUniform(head + ".ambient", m_Ambient);
	shader->SetUniform(head + ".diffuse", m_Diffuse);
	shader->SetUniform(head + ".specular", m_Specular);

	shader->SetUniform(head + ".position", m_Position);
	shader->SetUniform(head + ".constant", m_Constant);
	shader->SetUniform(head + ".linear", m_Linear);
	shader->SetUniform(head + ".quadratic", m_Quadratic);

	shader->Disable();
}