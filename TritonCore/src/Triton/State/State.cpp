#include "TRpch.h"
#include "State.h"

#include <glad\glad.h>

Triton::Singleton::State* Triton::Singleton::State::m_Instance = nullptr;


Triton::Singleton::State::State()
{

}

Triton::Singleton::State::~State()
{
}

void Triton::Singleton::State::Create()
{
	m_Instance = new State();
}

void Triton::Singleton::State::Destroy()
{
	delete m_Instance;
}

Triton::relay_ptr<Triton::Singleton::State> Triton::Singleton::State::GetInstance()
{
	return m_Instance;
}



void Triton::Singleton::State::ResetLights()
{
	m_count_dir = 0;
	m_count_point = 0;
	m_count_spot = 0;
}

unsigned int Triton::Singleton::State::AddLight(relay_ptr<Graphics::DirectionalLight> aLight)
{
	if (m_count_dir + 1 > TR_NR_MAX_DIR_LIGHTS)
	{
		TR_CORE_WARN("Maximum DIRECTIONAL light count exceeded!");
		TR_CORE_WARN("Setting the amount to 0!");
		m_count_dir = 0;
	}

	m_DirectionalLights[m_count_dir++] = aLight;

	return m_count_dir - 1;
}

unsigned int Triton::Singleton::State::AddLight(relay_ptr<Graphics::PointLight> aLight)
{
	if (m_count_point + 1 > TR_NR_MAX_POINT_LIGHTS)
	{
		TR_CORE_WARN("Maximum POINT light count exceeded!");
		TR_CORE_WARN("Setting the amount to 0!");
		m_count_point = 0;
	}

	m_PointLights[m_count_point++] = aLight;

	return m_count_point - 1;
}

unsigned int Triton::Singleton::State::AddLight(relay_ptr<Graphics::SpotLight> aLight)
{
	if (m_count_spot + 1 > TR_NR_MAX_SPOT_LIGHTS)
	{
		TR_CORE_WARN("Maximum SPOT light count exceeded!");
		TR_CORE_WARN("Setting the amount to 0!");
		m_count_spot = 0;
	}

	m_SpotLights[m_count_spot++] = aLight;

	return m_count_spot - 1;
}

void Triton::Singleton::State::UpdateLightCounts()
{
	m_CurrentShader->Enable();

	m_CurrentShader->SetUniform("num_of_dirlights", (int)m_count_dir);
	m_CurrentShader->SetUniform("num_of_pointlights", (int)m_count_point);
	m_CurrentShader->SetUniform("num_of_spotlights", (int)m_count_spot);

	m_CurrentShader->Disable();
}

void Triton::Singleton::State::UpdateUniforms()
{
	//Update counts
	UpdateLightCounts();


	m_CurrentShader->Enable();

	//Bind lights
	if (m_count_point > 0)
	for (unsigned int i = 0; i < m_count_point - 1; i++)
	{
		m_PointLights[i]->Bind(this);
	}

	if (m_count_dir > 0)
	for (unsigned int i = 0; i < m_count_dir - 1; i++)
	{
		m_DirectionalLights[i]->Bind(this);
	}

	if (m_count_spot > 0)
	for (unsigned int i = 0; i < m_count_spot - 1; i++)
	{
		m_SpotLights[i]->Bind(this);
	}

	m_CurrentShader->Disable();
}

void Triton::Singleton::State::BindShader(relay_ptr<Core::Shader> aShader)
{
	m_CurrentShader = aShader;
}
