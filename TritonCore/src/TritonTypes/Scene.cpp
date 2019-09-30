#include "TRpch.h"
#include "Scene.h"

namespace Triton
{
	Scene::Scene(std::string name)
		: m_name(name)
	{
	}

	IO::SceneData Scene::toRawData()
	{
		IO::SceneData result;

		result.Version = "Latest";
		result.Name = this->m_name;
		result.Assets = this->m_assets;
		result.Lights = this->m_lights;

		return result;
	}

	std::string Scene::getName() const
	{
		return m_name;
	}

	std::vector<std::string>& Scene::assets()
	{
		return m_assets;
	}

	Graphics::LightBuffer& Scene::lights()
	{
		return m_lights;
	}
}